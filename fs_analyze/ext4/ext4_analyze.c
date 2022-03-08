#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <getopt.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <inttypes.h>

#include "ext4_analyze.h"

#define DEFAULT_BLKSZ 4096
#define EXT4_MIN_BLOCK_SIZE 1024

static int dev;
static void *buf;

static uint64_t bmwidth=128;
static uint64_t bufsz=8192;

// called directly from cmd_table
static void analyze_superblock(int argc, char **argv);
static void analyze_group_descriptors(int argc, char **argv);
static void analyze_journal(int argc, char **argv);
static void analyze_itable(int argc, char **argv);
static void analyze_dentries(int argc, char **argv);
static void analyze_bm(int argc, char **argv);
static void set_bmwidth(int argc, char **argv);
static void set_bufsz(int argc, char **argv);
/*static void analyze_journal(int argc, char **argv);
static void analyze_allocation_file(int argc, char **argv);
static void analyze_catalog_file(int argc, char **argv);
static void analyze_attributes_file(int argc, char **argv);
static void analyze_extents_file(int argc, char **argv);
static void analyze_catalog_records(int argc, char **argv);
static void analyze_attributes_records(int argc, char **argv);
static void analyze_xattrs(int argc, char **argv);
static void analyze_btree_node(int argc, char **argv);
static void scan_btree(int argc, char **argv);
static void analyze_disk(int argc, char **argv);*/


// not called directly from cmd_table
static void _analyze_inode(struct ext2_inode_large *ino, uint32_t bsize);
/*static void _analyze_header_records(void *btn, HFSPlusForkData fd, uint32_t blksz, bool map);
static void _analyze_index_records(void *btn);
static void _analyze_leaf_records(void *btn);
static void _analyze_file_record(HFSPlusCatalogKey *key, HFSPlusCatalogFile *record);
static void _analyze_folder_record(HFSPlusCatalogKey *key, HFSPlusCatalogFolder *record);
static void _analyze_thread_record(HFSPlusCatalogKey *key, HFSPlusCatalogThread *record);
static void _analyze_xattr(HFSPlusAttrKey *key, HFSPlusAttrRecord *record);
static void _analyze_fork_data(HFSPlusForkData *fd);
static void _analyze_inline_attr_record(HFSPlusAttrData *data);
static void _analyze_attr_fork_record(HFSPlusAttrForkData *forkdata);
static void _analyze_bm(uint64_t addr, uint32_t blksz, int32_t bit);
static void __analyze_bm(uint8_t *bm, uint32_t bmsz, int32_t bit);
static void _analyze_btree_node(void *btn, uint32_t blksz);
static void _analyze_btree_file(HFSPlusForkData fd, uint32_t blksz, bool map, bool dump);*/

struct command {
	const char *name;
	void (*func)(int, char **);
};

static struct command cmd_table[] = {
	{"superblock", analyze_superblock},
    {"gdesc", analyze_group_descriptors},
    {"journal", analyze_journal},
    {"itable", analyze_itable},
    {"dentries", analyze_dentries},
    {"bm", analyze_bm},
	{"bmwidth", set_bmwidth},
	{"bufsz", set_bufsz},
	{NULL, NULL}
};

#define DOUBLE_QUOTE '"'
#define SINGLE_QUOTE '\''
#define BACK_SLASH   '\\'

static char **
build_argv(char *str, int *argc)
{
	int table_size = 16, _argc;
	char **argv;
	
	if (argc == NULL)
		argc = &_argc;
	
	*argc = 0;
	argv = (char **)calloc(table_size, sizeof(char *));
	
	if (argv == NULL)
		return NULL;
	
	while (*str) {
		// skip intervening white space //
		while (*str != '\0' && (*str == ' ' || *str == '\t' || *str == '\n'))
			str++;
		
		if (*str == '\0')
			break;
		
		if (*str == DOUBLE_QUOTE) {
			argv[*argc] = ++str;
			while (*str && *str != DOUBLE_QUOTE) {
				if (*str == BACK_SLASH)
					memmove(str, str + 1, strlen(str+1)+1); // copy everything down //
				str++;
			}
		} else if (*str == SINGLE_QUOTE) {
			argv[*argc] = ++str;
			while (*str && *str != SINGLE_QUOTE) {
				if (*str == BACK_SLASH)
					memmove(str, str + 1, strlen(str+1)+1); // copy everything down //
				str++;
			}
		} else {
			argv[*argc] = str;
			while (*str && *str != ' ' && *str != '\t' && *str != '\n') {
				if (*str == BACK_SLASH)
					memmove(str, str + 1, strlen(str+1)+1); // copy everything down //
				str++;
			}
		}
		
		if (*str != '\0')
			*str++ = '\0'; // chop the string //
		
		*argc = *argc + 1;
		if (*argc >= table_size - 1) {
			char **nargv;
			
			table_size = table_size * 2;
			nargv = (char **)calloc(table_size, sizeof(char *));
			
			if (nargv == NULL) { // drats! failure. //
				free(argv);
				return NULL;
			}
			
			memcpy(nargv, argv, (*argc) * sizeof(char *));
			free(argv);
			argv = nargv;
		}
	}
	
	return argv;
}

static void set_bmwidth(int argc, char **argv) {
	bmwidth = strtoull(argv[1], NULL, 0);
	return;
}

static void set_bufsz(int argc, char **argv) {
	bufsz = strtoull(argv[1], NULL, 0);
	buf = realloc(buf, bufsz);
	if (buf == NULL)
		printf("error realloc'ing buf to bufsz %" PRIu64 "\n", bufsz);
	else
		printf("realloc'd buf to bufsz %" PRIu64 "\n", bufsz);
	return;
}

// _<structure>_to_buf functions

static int _superblock_to_buf(void) {
	struct ext2_super_block *sb;
    uint32_t blksz;
	
	if (pread(dev, buf, bufsz, 0) != bufsz) {
		printf("pread() error.\n");
		return -1;
	}
	
	memmove(buf, buf + SUPERBLOCK_OFFSET, sizeof(struct ext2_super_block));
    
    sb = (struct ext2_super_block *)buf;
    blksz = 1 << (sb->s_log_block_size + 10);
    if (blksz > bufsz)
        printf("WARNING: fs block size (%" PRIu32 ") > bufsz (%" PRIu64 ")\n", blksz, bufsz);
    if (!(sb->s_feature_incompat & EXT4_FEATURE_INCOMPAT_64BIT)) {
        // we assume the high bits on 64-bit fields are used throughout this program
        printf("WARNING: !EXT4_FEATURE_INCOMPAT_64BIT currently unsupported\n");
    }
    
	return 0;
}

static int _group_descriptor_to_buf(uint32_t bsize, int descriptor_nr) {
    uint32_t blk_nr;
    off_t off;
    
    blk_nr = ((bsize == EXT4_MIN_BLOCK_SIZE) ? 2 : 1) + descriptor_nr;
    off = blk_nr * bsize;
    
    if (pread(dev, buf, bufsz, off) != bufsz) {
        printf("pread() error.\n");
        return -1;
    }
    
    return 0;
}

static int _itable_to_buf(uint32_t inum, struct ext2_inode_large **ino) {
    struct ext2_super_block *sb;
    struct ext4_group_desc *gd;
    uint8_t *itable;
    uint32_t bsize, inodes_per_group, inode_size, inodes_per_block, group_nr, group_off, desc_size, desc_per_block, desc_block_nr, desc_block_offset, bnum, block_off;
    
    if (_superblock_to_buf())
        return -1;
    
    sb = (struct ext2_super_block *)buf;
    
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    inodes_per_group = sb->s_inodes_per_group;
    inode_size = sb->s_inode_size;
    inodes_per_block = bsize / inode_size;
    group_nr = ((uint32_t)inum - 1) / inodes_per_group;
    group_off = ((uint32_t)inum - 1) % inodes_per_group;
    desc_size = sb->s_desc_size;
    desc_per_block = bsize / desc_size;
    desc_block_nr = group_nr / desc_per_block;
    desc_block_offset = group_nr % desc_per_block;
    
    if (_group_descriptor_to_buf(bsize, desc_block_nr))
        return -1;
    
    gd = (struct ext4_group_desc *)((char *)buf + (desc_block_offset * desc_size));
    bnum = ((uint64_t)gd->bg_inode_table_hi << 32 | gd->bg_inode_table) + (group_off / inodes_per_block);
    //printf("reading block %u\n", bnum);
    if (pread(dev, buf, bufsz, bnum * bsize) != bufsz) {
        printf("pread() error.\n");
        return -1;
    }
    
    itable = (uint8_t *)buf;
    
    block_off = group_off % inodes_per_block;
    //printf("block off %u\n", block_off);
    *ino = (struct ext2_inode_large *)(itable + (block_off * inode_size));

    return 0;
}

// analyze_<structure> functions

static void __analyze_bm(uint8_t *bm, uint32_t bmsz) {
    int ind=0, ones=0;
    
    while (ind < bmsz) {
        for (int i=0; i < (bmwidth / 2); i++) {
            for (int j=0; j < 8; j++) {
                if (bm[ind] & (1<<j))
                    ones++;
            }
            printf("%02x", bm[ind]);
            if (++ind >= bmsz)
                break;
        }
        printf("\n");
    }
    printf("bits set: %d\n", ones);
    
    printf("\n");
}

static void _analyze_bm(uint64_t addr, uint32_t bmsize, uint32_t blksz) {
    uint8_t *bm=NULL;
    
    bm = malloc(blksz);
    if (bm == NULL) {
        printf("malloc error\n");
        goto out;
    }
    
    if (pread(dev, bm, blksz, addr * blksz) != blksz) {
        printf("pread() error.\n");
        goto out;
    }
    
    __analyze_bm(bm, bmsize);
    
out:
    if (bm)
        free(bm);
    
    return;
}

// dump a bitmap block given any paddr
static void analyze_bm(int argc, char **argv) {
    struct ext2_super_block *sb;
    int64_t addr = -1;
    int32_t bmsize = -1, blksize = -1;
    int ch;
    
    struct option longopts[] = {
        { "addr",      required_argument,    NULL,    'a' },
        { "bmsize",    required_argument,    NULL,    's' },
        { NULL,            0,                NULL,    0 }
    };
    
    optind = 1;
    while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (ch) {
            case 'a':
                addr = strtoull(optarg, NULL, 0);
                break;
            case 's':
                bmsize = atoi(optarg);
                break;
            default:
                printf("usage: %s --addr <bm addr> [--bmsize <bmsize (in bits)>]\n", argv[0]);
                return;
        }
    }
    
    if (addr < 0) {
        printf("usage: %s --addr <bm addr> [--bmsize <bmsize (in bits)>]\n", argv[0]);
        return;
    }
    
    if (_superblock_to_buf())
        goto out;
    
    sb = (struct ext2_super_block *)buf;
    blksize = (int32_t)(1 << (10 + sb->s_log_block_size));
    
    if (bmsize < 0) {
        bmsize = blksize;
        printf("using volume blocksize (%d) as bmsize\n", blksize);
    } else {
        if (bmsize % 8)
            printf("WARNING: bmsize not divisible by 8, truncating now\n");
        bmsize /= 8;
    }
    _analyze_bm(addr, bmsize, blksize);
    
out:
    return;
}

static void analyze_dentries(int argc, char **argv) {
    struct ext2_super_block *sb;
    uint32_t bsize;
    struct ext2_dx_root_info *dx_root_info;
    struct ext2_dx_countlimit *dx_countlimit;
    uint16_t count;
    struct ext2_dx_entry *dx_entry;
    struct ext2_dir_entry_2 *de;
    char name[EXT2_NAME_LEN + 1];
    bool htree = false;
    int64_t addr = -1;
    int ch;
    
    struct option longopts[] = {
        { "addr",       required_argument,    NULL,    'a' },
        { "htree",      no_argument,          NULL,    'h' },
        { NULL,            0,                 NULL,     0 }
    };
    
    optind = 1;
    while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (ch) {
            case 'a':
                addr = strtoll(optarg, NULL, 0);;
                break;
            case 'h':
                htree = true;
                break;
            default:
                printf("usage: %s --addr <block addr> [--htree]\n", argv[0]);
                return;
        }
    }
    
    if (addr < 0) {
        printf("usage: %s --addr <block addr> [--htree]\n", argv[0]);
        goto out;
    }
    
    if (_superblock_to_buf())
        goto out;
    
    sb = (struct ext2_super_block *)buf;
    
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    
    if (pread(dev, buf, bufsz, addr * bsize) != bufsz) {
        printf("pread() error.\n");
        goto out;
    }
    
    if (htree) { // note: index nodes not yet supported
        dx_root_info = (struct ext2_dx_root_info *)((char *)buf + DX_ROOT_INFO_OFFSET);
        printf("dx root info:\n");
        printf("  hash_version: %" PRIu8 "\n", dx_root_info->hash_version);
        printf("  info_length: %" PRIu8 "\n", dx_root_info->info_length);
        printf("  indirect_levels: %" PRIu8 "\n", dx_root_info->indirect_levels);
        
        dx_countlimit = (struct ext2_dx_countlimit *)((char *)dx_root_info + dx_root_info->info_length);
        printf("limit: %" PRIu16 "\n", dx_countlimit->limit);
        printf("count: %" PRIu16 "\n", dx_countlimit->count);
        printf("block %" PRIu32 "\n", ((struct ext2_dx_entry *)dx_countlimit)->block);
        count = dx_countlimit->count;
        
        dx_entry = (struct ext2_dx_entry *)dx_countlimit;
        for (int i = 1; i < count; i++) {
            printf("dx_entry[%d]:\n", i);
            printf("  hash: 0x%" PRIX32 "\n", dx_entry[i].hash);
            printf("  block: %" PRIu32 "\n", dx_entry[i].block);
        }
    } else {
        printf("\n***** dentries at %" PRId64 " *****\n", addr);
        
        de = (struct ext2_dir_entry_2 *)buf;
        while (de->inode && ((char *)de < ((char *)buf + bufsz))) {
            printf("\ninode: %" PRIu32 "\n", de->inode);
            printf("rec_len: %" PRIu16 "\n", de->rec_len);
            printf("name_len: %" PRIu8 "\n", de->name_len);
            printf("file_type: %" PRIu8 "\n", de->file_type);
            memset(name, 0, sizeof(name));
            memcpy(name, de->name, de->name_len);
            printf("name: %s\n", name);
            de = (struct ext2_dir_entry_2 *)((char *)de + de->rec_len);
        }
    }
    
    printf("\n");
    
out:
    return;
}

static void _dump_extent_tree_nodes(struct ext3_extent_header *eh, uint32_t bsize) {
    uint16_t nentries;
    struct ext3_extent_idx *ei;
    struct ext3_extent *ex;
    uint64_t *nodes;
    
    printf("  extent header:\n");
    printf("    eh_magic: 0x%" PRIX16 "\n", eh->eh_magic);
    printf("    eh_entries: %" PRIu16 "\n", eh->eh_entries);
    printf("    eh_max: %" PRIu16 "\n", eh->eh_max);
    printf("    eh_depth: %" PRIu16 "\n", eh->eh_depth);
    printf("    eh_generation: %" PRIu32 "\n", eh->eh_generation);
    if (eh->eh_depth) {
        ei = (struct ext3_extent_idx *)((char *)eh + sizeof(struct ext3_extent_header));
        nodes = malloc(eh->eh_entries * sizeof(uint64_t));
        if (!nodes) {
            printf("malloc error\n");
            goto out;
        }
        for (int i = 0; i < eh->eh_entries; i++, ei++) {
            printf("  index %d:\n", i);
            printf("    ei_block: %" PRIu32 "\n", ei->ei_block);
            printf("    ei_leaf: %" PRIu64 "\n", ((uint64_t)ei->ei_leaf_hi << 32) | ei->ei_leaf);
            nodes[i] = ((uint64_t)ei->ei_leaf_hi << 32) | ei->ei_leaf;
        }
        printf("\n");
        nentries = eh->eh_entries;
        for (int i = 0; i < nentries; i++) {
            if (pread(dev, buf, bufsz, nodes[i] * bsize) != bufsz) {
                printf("pread() error.\n");
                goto out;
            }
            printf("-- extent tree node @ block %" PRIu64 " --\n", nodes[i]);
            _dump_extent_tree_nodes((struct ext3_extent_header *)buf, bsize);
        }
        free(nodes);
    } else { // leaf node
        ex = (struct ext3_extent *)((char *)eh + sizeof(struct ext3_extent_header));
        for (int i = 0; i < eh->eh_entries; i++, ex++) {
            printf("  extent %d:\n", i);
            printf("    ee_block: %" PRIu32 "\n", ex->ee_block);
            printf("    ee_len: %" PRIu16 "\n", ex->ee_len);
            printf("    ee_start: %" PRIu64 "\n", ((uint64_t)ex->ee_start_hi << 32) | ex->ee_start);
        }
        printf("\n");
    }
    
out:
    return;
}

static void _analyze_inode(struct ext2_inode_large *ino, uint32_t bsize){
    struct ext3_extent_header *eh;
    
    printf("i_mode: 0x%" PRIX16 "\n", ino->i_mode);
    printf("i_size: %" PRIu64 "\n", ((uint64_t)ino->i_size_high << 32) | ino->i_size);
    printf("i_links_count: %" PRIu16 "\n", ino->i_links_count);
    printf("i_blocks: %" PRIu64 "\n", ((uint64_t)ino->osd2.linux2.l_i_blocks_hi << 32) | ino->i_blocks);
    printf("i_flags: 0x%" PRIX32 "\n", ino->i_flags);
    printf("i_blocks:\n");
    if (ino->i_flags & EXT4_EXTENTS_FL) { // dump the extents
        eh = (struct ext3_extent_header *)(ino->i_block);
        _dump_extent_tree_nodes(eh, bsize);
    }
    
out:
    return;
}

static void analyze_itable(int argc, char **argv) {
    struct ext2_super_block *sb;
    struct ext2_inode_large *ino;
    bool dump = false;
    uint32_t bsize;
    int64_t inum = -1;
    int ch;
    
    struct option longopts[] = {
        { "dump",       required_argument,    NULL,    'd' },
        { "inum",       required_argument,    NULL,    'i' },
        { NULL,            0,                 NULL,     0 }
    };
    
    optind = 1;
    while ((ch = getopt_long(argc, argv, "", longopts, NULL)) != -1) {
        switch (ch) {
            case 'd':
                dump = true;
                break;
            case 'i':
                inum = strtoll(optarg, NULL, 0);
                break;
            default:
                printf("usage: %s --dump | --inum <inum>\n", argv[0]);
                return;
        }
    }
    
    if (!dump && (inum < 0)) {
        printf("usage: %s --dump | --inum <inum>\n", argv[0]);
        goto out;
    }
    
    if (dump) {
        printf("dump not yet supported\n");
        goto out;
    }
    
    if (_superblock_to_buf())
        goto out;
    
    sb = (struct ext2_super_block *)buf;
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    
    if (_itable_to_buf((uint32_t)inum, &ino))
        goto out;
        
    printf("\n--- inum %" PRIu64 " ---\n", (uint64_t)inum);
    _analyze_inode(ino, bsize);
    printf("\n");
    
out:
    return;
}

static void analyze_journal(int argc, char **argv) {
    struct ext2_super_block *sb;
    struct ext2_inode_large *ino;
    struct ext3_extent_header *eh;
    struct ext3_extent *ex;
    journal_superblock_t *jsb;
    journal_header_t *jh;
    journal_block_tag3_t *bt, *nbt;
    int tag_nr = 0;
    uint32_t bsize, jinum, next_expected_sequence;
    bool jnl_needs_recovery;
    uint64_t bnum, firstblock, currblock;
    
    if (_superblock_to_buf())
        goto out;
    
    sb = (struct ext2_super_block *)buf;
    
    if (!(sb->s_feature_compat & EXT3_FEATURE_COMPAT_HAS_JOURNAL)) {
        printf("no journal (!EXT3_FEATURE_COMPAT_HAS_JOURNAL)\n");
        goto out;
    }
    
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    jinum = sb->s_journal_inum;
    jnl_needs_recovery = (sb->s_feature_incompat & EXT3_FEATURE_INCOMPAT_RECOVER);
    
    if (_itable_to_buf(jinum, &ino))
        goto out;
    
    printf("\n***** journal *****\n");
    
    printf("\njournal inum: %" PRIu32 "\n", jinum);
    
    printf("\njournal inode:\n");
    _analyze_inode(ino, bsize);
    printf("\n");
    
    eh = (struct ext3_extent_header *)(ino->i_block);
    
    if (eh->eh_entries > 1) {
        printf("journal with more than 1 extent unsupported\n");
        goto out;
    }
    
    ex = (struct ext3_extent *)((char *)eh + sizeof(struct ext3_extent_header));
    bnum = ((uint64_t)ex->ee_start_hi << 32) | ex->ee_start;
    firstblock = currblock = bnum;
    
    // read in the journal superblock
    if (pread(dev, buf, bufsz, bnum * bsize) != bufsz) {
        printf("pread() error.\n");
        goto out;
    }
        
    jsb = (journal_superblock_t *)buf;
    
    printf("journal superblock:\n");
    printf("s_header.h_magic: 0x%" PRIX32 "\n", ntohl(jsb->s_header.h_magic));
    printf("s_header.h_blocktype: %" PRIu32 "\n", ntohl(jsb->s_header.h_blocktype));
    printf("s_header.h_sequence: %" PRIu32 "\n", ntohl(jsb->s_header.h_sequence));
    printf("s_blocksize: %" PRIu32 "\n", ntohl(jsb->s_blocksize));
    printf("s_maxlen: %" PRIu32 "\n", ntohl(jsb->s_maxlen));
    printf("s_first: %" PRIu32 "\n", ntohl(jsb->s_first));
    printf("s_sequence: %" PRIu32 "\n", ntohl(jsb->s_sequence));
    printf("s_start: %" PRIu32 "\n", ntohl(jsb->s_start));
    printf("s_feature_compat: 0x%" PRIX32 "\n", ntohl(jsb->s_feature_compat));
    printf("s_feature_incompat: 0x%" PRIX32 "\n", ntohl(jsb->s_feature_incompat));
    printf("s_feature_ro_compat: 0x%" PRIX32 "\n", ntohl(jsb->s_feature_ro_compat));
    printf("\n");
    
    if (!(ntohl(jsb->s_feature_incompat) & JBD2_FEATURE_INCOMPAT_CSUM_V3) ||
        !(ntohl(jsb->s_feature_incompat) & JBD2_FEATURE_INCOMPAT_64BIT)) {
        printf("!JBD2_FEATURE_INCOMPAT_CSUM_V3 || !JBD2_FEATURE_INCOMPAT_64BIT not supported\n");
        goto out;
    }
    
    if (!jnl_needs_recovery) {
        printf("** journal is clean **\n");
        goto out;
    }
    
    currblock = firstblock + ntohl(jsb->s_start);
    next_expected_sequence = ntohl(jsb->s_sequence);
    
    while (1) {
        if (pread(dev, buf, bufsz, currblock * bsize) != bufsz) {
            printf("pread() error.\n");
            goto out;
        }
        
        jh = (journal_header_t *)buf;
        if (ntohl(jh->h_sequence) != next_expected_sequence) // we're done
            break;
        
        printf("*** journal block %" PRIu64 " ***\n", currblock);
        printf("h_magic: 0x%" PRIX32 "\n", ntohl(jh->h_magic));
        printf("h_blocktype: %" PRIu32 "\n", ntohl(jh->h_blocktype));
        printf("h_sequence: %" PRIu32 "\n", ntohl(jh->h_sequence));
        
        switch (ntohl(jh->h_blocktype)) {
            case JBD2_DESCRIPTOR_BLOCK:
                nbt = (journal_block_tag3_t *)((char *)jh + sizeof(journal_header_t));
                printf("tags:\n");
                do {
                    bt = nbt;
                    printf("  tag[%d]: t_blocknr: %" PRIu64 " t_flags: 0x%" PRIX32 "\n", tag_nr,
                        ((uint64_t)ntohl(bt->t_blocknr_high) << 32) | ntohl(bt->t_blocknr),
                        ntohl(bt->t_flags));
                    tag_nr++;
                    nbt++;
                    if (!(ntohl(bt->t_flags) & JBD2_FLAG_SAME_UUID))
                        nbt = (journal_block_tag3_t *)((char *)nbt + UUID_SIZE);
                    currblock++;
                } while (!(ntohl(bt->t_flags) & JBD2_FLAG_LAST_TAG));
                printf("\n");
                break;
            case JBD2_COMMIT_BLOCK:
                next_expected_sequence++;
                break;
            case JBD2_SUPERBLOCK_V1:
            case JBD2_SUPERBLOCK_V2:
            case JBD2_REVOKE_BLOCK:
            case JBD2_FC_BLOCK:
                break;
        }
        currblock++;
    }
    
out:
    printf("\n");
    
    return;
}

static void analyze_group_descriptors(int argc, char **argv) {
    struct ext2_super_block *sb;
    struct ext4_group_desc *gd;
    uint32_t bsize, nr_groups, desc_size, desc_per_block, nr_descriptor_blocks, this_block_start, nr_desc_this_block;
    
    if (_superblock_to_buf())
        goto out;
    
    sb = (struct ext2_super_block *)buf;
    
    printf("\n***** group descriptors *****\n");
    
    // below calculations are from ext4_fill_super code
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    nr_groups = (sb->s_blocks_count - sb->s_first_data_block + sb->s_blocks_per_group - 1) / sb->s_blocks_per_group;
    desc_size = sb->s_desc_size;
    desc_per_block = bsize / desc_size;
    nr_descriptor_blocks = (nr_groups + desc_per_block - 1) / desc_per_block;
    printf("number of descriptor blocks: %" PRIu32 "\n", nr_descriptor_blocks);
    
    if (sb->s_feature_incompat & EXT2_FEATURE_INCOMPAT_META_BG) {
        printf("EXT2_FEATURE_INCOMPAT_META_BG currently unsupported\n");
        goto out;
    }
    
    for (int i = 0; i < nr_descriptor_blocks; i++) {
        if (_group_descriptor_to_buf(bsize, i))
            goto out;
        
        gd = (struct ext4_group_desc *)buf;
        
        this_block_start = i * desc_per_block;
        nr_desc_this_block = desc_per_block;
        if (this_block_start + desc_per_block > nr_groups)
            nr_desc_this_block = nr_groups - this_block_start;
        
        for (int j = 0; j < nr_desc_this_block; j++) {
            printf("\n--- group descriptor %d ---\n", i * desc_per_block + j);

            printf("bg_block_bitmap: %" PRIu64 "\n", ((uint64_t)gd->bg_block_bitmap_hi << 32) | gd->bg_block_bitmap);
            printf("bg_inode_bitmap: %" PRIu64 "\n", ((uint64_t)gd->bg_inode_bitmap_hi << 32) | gd->bg_inode_bitmap);
            printf("bg_inode_table: %" PRIu64 "\n", ((uint64_t)gd->bg_inode_table_hi << 32) | gd->bg_inode_table);
            printf("bg_free_blocks_count: %" PRIu32 "\n", ((uint32_t)gd->bg_free_blocks_count_hi << 16) | gd->bg_free_blocks_count);
            printf("bg_free_inodes_count: %" PRIu32 "\n", ((uint32_t)gd->bg_free_inodes_count_hi << 16) | gd->bg_free_inodes_count);
            printf("bg_used_dirs_count: %" PRIu32 "\n", ((uint32_t)gd->bg_used_dirs_count_hi << 16) | gd->bg_used_dirs_count);
            printf("bg_flags: 0x%" PRIX16 "\n", gd->bg_flags);
            gd = (struct ext4_group_desc *)((char *)gd + desc_size);
        }
    }
    
    printf("\n");
    
out:
    return;
}

static void analyze_superblock(int argc, char **argv) {
	struct ext2_super_block *sb;
    uint32_t bsize, nr_groups;
	
	if (_superblock_to_buf())
		goto out;
	
	sb = (struct ext2_super_block *)buf;
    
	printf("\n***** super block *****\n");
	
    printf("s_inodes_count: %" PRIu32 "\n", sb->s_inodes_count);
    printf("s_blocks_count: %" PRIu64 "\n", ((uint64_t)sb->s_blocks_count_hi << 32) | sb->s_blocks_count);
    printf("s_r_blocks_count: %" PRIu64 "\n", ((uint64_t)sb->s_r_blocks_count_hi << 32) | sb->s_r_blocks_count);
    printf("s_free_blocks_count: %" PRIu64 "\n", ((uint64_t)sb->s_free_blocks_hi << 32) | sb->s_free_blocks_count);
    printf("s_free_inodes_count: %" PRIu32 "\n", sb->s_free_inodes_count);
    printf("s_first_data_block: %" PRIu32 "\n", sb->s_first_data_block);
    
    bsize = (uint32_t)(1 << (10 + sb->s_log_block_size));
    printf("s_log_block_size: %" PRIu32 " (block size %" PRIu32 ")\n", sb->s_log_block_size, bsize);
    
    printf("s_log_cluster_size: %" PRIu32 " (cluster size %" PRIu32 ")\n", sb->s_log_cluster_size, (uint32_t)(1 << (10 + sb->s_log_cluster_size)));
    printf("s_blocks_per_group: %" PRIu32 "\n", sb->s_blocks_per_group);
    printf("s_clusters_per_group: %" PRIu32 "\n", sb->s_clusters_per_group);
    printf("s_inodes_per_group: %" PRIu32 "\n", sb->s_inodes_per_group);
	printf("s_magic: 0x%" PRIX16 "\n", sb->s_magic);
    printf("s_first_ino: %" PRIu32 "\n", sb->s_first_ino);
    printf("s_inode_size: %" PRIu16 "\n", sb->s_inode_size);
    printf("s_block_group_nr: %" PRIu16 "\n", sb->s_block_group_nr);
    printf("s_feature_compat: 0x%" PRIX32 "\n", sb->s_feature_compat);
    printf("s_feature_incompat: 0x%" PRIX32 "\n", sb->s_feature_incompat);
    printf("s_feature_ro_compat: 0x%" PRIX32 "\n", sb->s_feature_ro_compat);
    printf("s_volume_name: %s\n", (char *)(sb->s_volume_name));
    printf("s_desc_size: %" PRIu16 "\n", sb->s_desc_size);
    printf("s_first_meta_bg: %" PRIu32 "\n", sb->s_first_meta_bg);
    printf("s_log_groups_per_flex: %" PRIu8 " (groups per flex %" PRIu32 ")\n", sb->s_log_groups_per_flex, (uint32_t)(1 << sb->s_log_groups_per_flex));
    printf("s_journal_inum: %" PRIu32 "\n", sb->s_journal_inum);
	printf("\n");
    
    nr_groups = (sb->s_blocks_count - sb->s_first_data_block + sb->s_blocks_per_group - 1) / sb->s_blocks_per_group;
    printf("number of groups: %" PRIu32 "\n", nr_groups);
    printf("\n");
    
out:
	return;
}

int ext4_analyze(int argc, char **argv) {
	char *device_path;
	char in[256] = { 0 };
	int _argc;
	char **_argv = NULL;
	struct command *cmd;
	int error=0;
	
	if (argv[1])
		device_path = argv[1];
    else {
        printf("usage: %s <block device>\n", argv[0]);
        return -1;
    }
        
	dev = open(device_path, O_RDONLY);
	if (dev == -1) {
		printf("open error\n");
		return -1;
	}
	
	buf = malloc(bufsz);
	if (buf == NULL) {
		printf("malloc error\n");
		goto out;
	}
	
	while (1) {
		printf(">>");
		gets(in);
		
		if (strcmp(in, "quit") == 0)
			break;
		
		_argv = build_argv(in, &_argc);
		if (_argv == NULL || _argc == 0)
			continue;
		
		int i;
		for (i=0, cmd = &cmd_table[i]; cmd->name; cmd = &cmd_table[++i]) {
			if (strcmp(cmd->name, _argv[0]) == 0) {
				cmd->func(_argc, _argv);
				break;
			}
		}
		
		if (!cmd->name) {
			printf("unknown command: %s\n", _argv[0]);
			printf("known commands:\n");
			for (i=0, cmd = &cmd_table[i]; cmd->name; cmd = &cmd_table[++i]) {
				printf("%s\n", cmd->name);
			}
		}
		
		
		free(_argv);
	}
	
out:
	if (buf)
		free(buf);
	close(dev);
	
	return error;
}
