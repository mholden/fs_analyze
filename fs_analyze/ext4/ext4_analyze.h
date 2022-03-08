#ifndef _EXT4_ANALYZE_H_
#define _EXT4_ANALYZE_H_



int ext4_analyze(int argc, char **argv);

#define DX_ROOT_INFO_OFFSET 0x18

/*
 * e2fsprogs lib/ext2fs/ext2_fs.h:
 */

/*
 *  linux/include/linux/ext2_fs.h
 *
 * Copyright (C) 1992, 1993, 1994, 1995
 * Remy Card (card@masi.ibp.fr)
 * Laboratoire MASI - Institut Blaise Pascal
 * Universite Pierre et Marie Curie (Paris VI)
 *
 *  from
 *
 *  linux/include/linux/minix_fs.h
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

#ifndef _LINUX_EXT2_FS_H
#define _LINUX_EXT2_FS_H

#include <ext2fs/ext2_types.h>        /* Changed from linux/types.h */


#ifndef __GNUC_PREREQ
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
    ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#ifndef __nonstring
#ifdef __has_attribute
#if __has_attribute(__nonstring__)
#define __nonstring                    __attribute__((__nonstring__))
#else
#define __nonstring
#endif /* __has_attribute(__nonstring__) */
#else
# define __nonstring
#endif /* __has_attribute */
#endif /* __nonstring */

/*
 * The second extended filesystem constants/structures
 */

/*
 * Define EXT2FS_DEBUG to produce debug messages
 */
#undef EXT2FS_DEBUG

/*
 * Define EXT2_PREALLOCATE to preallocate data blocks for expanding files
 */
#define EXT2_PREALLOCATE
#define EXT2_DEFAULT_PREALLOC_BLOCKS    8

/*
 * The second extended file system version
 */
#define EXT2FS_DATE        "95/08/09"
#define EXT2FS_VERSION        "0.5b"

/*
 * Special inode numbers
 */
#define EXT2_BAD_INO         1    /* Bad blocks inode */
#define EXT2_ROOT_INO         2    /* Root inode */
#define EXT4_USR_QUOTA_INO     3    /* User quota inode */
#define EXT4_GRP_QUOTA_INO     4    /* Group quota inode */
#define EXT2_BOOT_LOADER_INO     5    /* Boot loader inode */
#define EXT2_UNDEL_DIR_INO     6    /* Undelete directory inode */
#define EXT2_RESIZE_INO         7    /* Reserved group descriptors inode */
#define EXT2_JOURNAL_INO     8    /* Journal inode */
#define EXT2_EXCLUDE_INO     9    /* The "exclude" inode, for snapshots */
#define EXT4_REPLICA_INO    10    /* Used by non-upstream feature */

/* First non-reserved inode for old ext2 filesystems */
#define EXT2_GOOD_OLD_FIRST_INO    11

/*
 * The second extended file system magic number
 */
#define EXT2_SUPER_MAGIC    0xEF53

#ifdef __KERNEL__
#define EXT2_SB(sb)    (&((sb)->u.ext2_sb))
#else
/* Assume that user mode programs are passing in an ext2fs superblock, not
 * a kernel struct super_block.  This will allow us to call the feature-test
 * macros from user land. */
#define EXT2_SB(sb)    (sb)
#endif

/*
 * Maximal count of links to a file
 */
#define EXT2_LINK_MAX        65000

/*
 * Macro-instructions used to manage several block sizes
 */
#define EXT2_MIN_BLOCK_LOG_SIZE        10    /* 1024 */
#define EXT2_MAX_BLOCK_LOG_SIZE        16    /* 65536 */
#define EXT2_MIN_BLOCK_SIZE    (1 << EXT2_MIN_BLOCK_LOG_SIZE)
#define EXT2_MAX_BLOCK_SIZE    (1 << EXT2_MAX_BLOCK_LOG_SIZE)
#ifdef __KERNEL__
#define EXT2_BLOCK_SIZE(s)    ((s)->s_blocksize)
#define EXT2_BLOCK_SIZE_BITS(s)    ((s)->s_blocksize_bits)
#define EXT2_ADDR_PER_BLOCK_BITS(s)    (EXT2_SB(s)->addr_per_block_bits)
#define EXT2_INODE_SIZE(s)    (EXT2_SB(s)->s_inode_size)
#define EXT2_FIRST_INO(s)    (EXT2_SB(s)->s_first_ino)
#else
#define EXT2_BLOCK_SIZE(s)    (EXT2_MIN_BLOCK_SIZE << (s)->s_log_block_size)
#define EXT2_BLOCK_SIZE_BITS(s)    ((s)->s_log_block_size + 10)
#define EXT2_INODE_SIZE(s)    (((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
                 EXT2_GOOD_OLD_INODE_SIZE : (s)->s_inode_size)
#define EXT2_FIRST_INO(s)    (((s)->s_rev_level == EXT2_GOOD_OLD_REV) ? \
                 EXT2_GOOD_OLD_FIRST_INO : (s)->s_first_ino)
#endif
#define EXT2_ADDR_PER_BLOCK(s)    (EXT2_BLOCK_SIZE(s) / sizeof(__u32))

/*
 * Macro-instructions used to manage allocation clusters
 */
#define EXT2_MIN_CLUSTER_LOG_SIZE    EXT2_MIN_BLOCK_LOG_SIZE
#define EXT2_MAX_CLUSTER_LOG_SIZE    29    /* 512MB  */
#define EXT2_MIN_CLUSTER_SIZE        EXT2_MIN_BLOCK_SIZE
#define EXT2_MAX_CLUSTER_SIZE        (1 << EXT2_MAX_CLUSTER_LOG_SIZE)
#define EXT2_CLUSTER_SIZE(s)        (EXT2_MIN_BLOCK_SIZE << \
                        (s)->s_log_cluster_size)
#define EXT2_CLUSTER_SIZE_BITS(s)    ((s)->s_log_cluster_size + 10)

/*
 * Macro-instructions used to manage fragments
 *
 * Note: for backwards compatibility only, for the dump program.
 * Ext2/3/4 will never support fragments....
 */
#define EXT2_MIN_FRAG_SIZE              EXT2_MIN_BLOCK_SIZE
#define EXT2_MAX_FRAG_SIZE              EXT2_MAX_BLOCK_SIZE
#define EXT2_MIN_FRAG_LOG_SIZE          EXT2_MIN_BLOCK_LOG_SIZE
#define EXT2_FRAG_SIZE(s)        EXT2_BLOCK_SIZE(s)
#define EXT2_FRAGS_PER_BLOCK(s)        1

/*
 * ACL structures
 */
struct ext2_acl_header    /* Header of Access Control Lists */
{
    __u32    aclh_size;
    __u32    aclh_file_count;
    __u32    aclh_acle_count;
    __u32    aclh_first_acle;
};

struct ext2_acl_entry    /* Access Control List Entry */
{
    __u32    acle_size;
    __u16    acle_perms;    /* Access permissions */
    __u16    acle_type;    /* Type of entry */
    __u16    acle_tag;    /* User or group identity */
    __u16    acle_pad1;
    __u32    acle_next;    /* Pointer on next entry for the */
                    /* same inode or on next free entry */
};

/*
 * Structure of a blocks group descriptor
 */
struct ext2_group_desc
{
    __u32    bg_block_bitmap;    /* Blocks bitmap block */
    __u32    bg_inode_bitmap;    /* Inodes bitmap block */
    __u32    bg_inode_table;        /* Inodes table block */
    __u16    bg_free_blocks_count;    /* Free blocks count */
    __u16    bg_free_inodes_count;    /* Free inodes count */
    __u16    bg_used_dirs_count;    /* Directories count */
    __u16    bg_flags;
    __u32    bg_exclude_bitmap_lo;    /* Exclude bitmap for snapshots */
    __u16    bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
    __u16    bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
    __u16    bg_itable_unused;    /* Unused inodes count */
    __u16    bg_checksum;        /* crc16(s_uuid+group_num+group_desc)*/
};

/*
 * Structure of a blocks group descriptor
 */
struct ext4_group_desc
{
    __u32    bg_block_bitmap;    /* Blocks bitmap block */
    __u32    bg_inode_bitmap;    /* Inodes bitmap block */
    __u32    bg_inode_table;        /* Inodes table block */
    __u16    bg_free_blocks_count;    /* Free blocks count */
    __u16    bg_free_inodes_count;    /* Free inodes count */
    __u16    bg_used_dirs_count;    /* Directories count */
    __u16    bg_flags;        /* EXT4_BG_flags (INODE_UNINIT, etc) */
    __u32    bg_exclude_bitmap_lo;    /* Exclude bitmap for snapshots */
    __u16    bg_block_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
    __u16    bg_inode_bitmap_csum_lo;/* crc32c(s_uuid+grp_num+bitmap) LSB */
    __u16    bg_itable_unused;    /* Unused inodes count */
    __u16    bg_checksum;        /* crc16(sb_uuid+group+desc) */
    __u32    bg_block_bitmap_hi;    /* Blocks bitmap block MSB */
    __u32    bg_inode_bitmap_hi;    /* Inodes bitmap block MSB */
    __u32    bg_inode_table_hi;    /* Inodes table block MSB */
    __u16    bg_free_blocks_count_hi;/* Free blocks count MSB */
    __u16    bg_free_inodes_count_hi;/* Free inodes count MSB */
    __u16    bg_used_dirs_count_hi;    /* Directories count MSB */
    __u16    bg_itable_unused_hi;    /* Unused inodes count MSB */
    __u32    bg_exclude_bitmap_hi;    /* Exclude bitmap block MSB */
    __u16    bg_block_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
    __u16    bg_inode_bitmap_csum_hi;/* crc32c(s_uuid+grp_num+bitmap) MSB */
    __u32    bg_reserved;
};

#define EXT4_BG_INODE_BITMAP_CSUM_HI_END    \
    (offsetof(struct ext4_group_desc, bg_inode_bitmap_csum_hi) + \
     sizeof(__u16))
#define EXT4_BG_BLOCK_BITMAP_CSUM_HI_LOCATION    \
    (offsetof(struct ext4_group_desc, bg_block_bitmap_csum_hi) + \
     sizeof(__u16))

#define EXT2_BG_INODE_UNINIT    0x0001 /* Inode table/bitmap not initialized */
#define EXT2_BG_BLOCK_UNINIT    0x0002 /* Block bitmap not initialized */
#define EXT2_BG_INODE_ZEROED    0x0004 /* On-disk itable initialized to zero */

/*
 * Data structures used by the directory indexing feature
 *
 * Note: all of the multibyte integer fields are little endian.
 */

/*
 * Note: dx_root_info is laid out so that if it should somehow get
 * overlaid by a dirent the two low bits of the hash version will be
 * zero.  Therefore, the hash version mod 4 should never be 0.
 * Sincerely, the paranoia department.
 */
struct ext2_dx_root_info {
    __u32 reserved_zero;
    __u8 hash_version; /* 0 now, 1 at release */
    __u8 info_length; /* 8 */
    __u8 indirect_levels;
    __u8 unused_flags;
};

#define EXT2_HASH_LEGACY        0
#define EXT2_HASH_HALF_MD4        1
#define EXT2_HASH_TEA            2
#define EXT2_HASH_LEGACY_UNSIGNED    3 /* reserved for userspace lib */
#define EXT2_HASH_HALF_MD4_UNSIGNED    4 /* reserved for userspace lib */
#define EXT2_HASH_TEA_UNSIGNED        5 /* reserved for userspace lib */
#define EXT2_HASH_SIPHASH        6

#define EXT2_HASH_FLAG_INCOMPAT    0x1

#define EXT4_DX_BLOCK_MASK 0x0fffffff

struct ext2_dx_entry {
    __le32 hash;
    __le32 block;
};

struct ext2_dx_countlimit {
    __le16 limit;
    __le16 count;
};

/*
 * This goes at the end of each htree block.
 */
struct ext2_dx_tail {
    __le32 dt_reserved;
    __le32 dt_checksum;    /* crc32c(uuid+inum+dxblock) */
};

/*
 * Macro-instructions used to manage group descriptors
 */
#define EXT2_MIN_DESC_SIZE             32
#define EXT2_MIN_DESC_SIZE_64BIT       64
#define EXT2_MAX_DESC_SIZE             EXT2_MIN_BLOCK_SIZE
#define EXT2_DESC_SIZE(s)                                                \
    (ext2fs_has_feature_64bit(s) ? (s)->s_desc_size : EXT2_MIN_DESC_SIZE)

#define EXT2_BLOCKS_PER_GROUP(s)    (EXT2_SB(s)->s_blocks_per_group)
#define EXT2_INODES_PER_GROUP(s)    (EXT2_SB(s)->s_inodes_per_group)
#define EXT2_CLUSTERS_PER_GROUP(s)    (EXT2_SB(s)->s_clusters_per_group)
#define EXT2_INODES_PER_BLOCK(s)    (EXT2_BLOCK_SIZE(s)/EXT2_INODE_SIZE(s))
/* limits imposed by 16-bit value gd_free_{blocks,inode}_count */
#define EXT2_MAX_BLOCKS_PER_GROUP(s)    ((((unsigned) 1 << 16) - 8) *    \
                     (EXT2_CLUSTER_SIZE(s) / \
                      EXT2_BLOCK_SIZE(s)))
#define EXT2_MAX_CLUSTERS_PER_GROUP(s)    (((unsigned) 1 << 16) - 8)
#define EXT2_MAX_INODES_PER_GROUP(s)    (((unsigned) 1 << 16) - \
                     EXT2_INODES_PER_BLOCK(s))
#ifdef __KERNEL__
#define EXT2_DESC_PER_BLOCK(s)        (EXT2_SB(s)->s_desc_per_block)
#define EXT2_DESC_PER_BLOCK_BITS(s)    (EXT2_SB(s)->s_desc_per_block_bits)
#else
#define EXT2_DESC_PER_BLOCK(s)        (EXT2_BLOCK_SIZE(s) / EXT2_DESC_SIZE(s))
#endif

#define EXT2_GROUPS_TO_BLOCKS(s, g)   ((blk64_t) EXT2_BLOCKS_PER_GROUP(s) * \
                       (g))
#define EXT2_GROUPS_TO_CLUSTERS(s, g) ((blk64_t) EXT2_CLUSTERS_PER_GROUP(s) * \
                       (g))

/*
 * Constants relative to the data blocks
 */
#define EXT2_NDIR_BLOCKS        12
#define EXT2_IND_BLOCK            EXT2_NDIR_BLOCKS
#define EXT2_DIND_BLOCK            (EXT2_IND_BLOCK + 1)
#define EXT2_TIND_BLOCK            (EXT2_DIND_BLOCK + 1)
#define EXT2_N_BLOCKS            (EXT2_TIND_BLOCK + 1)

/*
 * Inode flags
 */
#define EXT2_SECRM_FL            0x00000001 /* Secure deletion */
#define EXT2_UNRM_FL            0x00000002 /* Undelete */
#define EXT2_COMPR_FL            0x00000004 /* Compress file */
#define EXT2_SYNC_FL            0x00000008 /* Synchronous updates */
#define EXT2_IMMUTABLE_FL        0x00000010 /* Immutable file */
#define EXT2_APPEND_FL            0x00000020 /* writes to file may only append */
#define EXT2_NODUMP_FL            0x00000040 /* do not dump file */
#define EXT2_NOATIME_FL            0x00000080 /* do not update atime */
/* Reserved for compression usage... */
#define EXT2_DIRTY_FL            0x00000100
#define EXT2_COMPRBLK_FL        0x00000200 /* One or more compressed clusters */
#define EXT2_NOCOMPR_FL            0x00000400 /* Access raw compressed data */
    /* nb: was previously EXT2_ECOMPR_FL */
#define EXT4_ENCRYPT_FL            0x00000800 /* encrypted inode */
/* End compression flags --- maybe not all used */
#define EXT2_BTREE_FL            0x00001000 /* btree format dir */
#define EXT2_INDEX_FL            0x00001000 /* hash-indexed directory */
#define EXT2_IMAGIC_FL            0x00002000
#define EXT3_JOURNAL_DATA_FL        0x00004000 /* file data should be journaled */
#define EXT2_NOTAIL_FL            0x00008000 /* file tail should not be merged */
#define EXT2_DIRSYNC_FL         0x00010000 /* Synchronous directory modifications */
#define EXT2_TOPDIR_FL            0x00020000 /* Top of directory hierarchies*/
#define EXT4_HUGE_FILE_FL               0x00040000 /* Set to each huge file */
#define EXT4_EXTENTS_FL         0x00080000 /* Inode uses extents */
#define EXT4_VERITY_FL            0x00100000 /* Verity protected inode */
#define EXT4_EA_INODE_FL            0x00200000 /* Inode used for large EA */
/* EXT4_EOFBLOCKS_FL 0x00400000 was here */
#define FS_NOCOW_FL            0x00800000 /* Do not cow file */
#define EXT4_SNAPFILE_FL        0x01000000  /* Inode is a snapshot */
#define FS_DAX_FL            0x02000000 /* Inode is DAX */
#define EXT4_SNAPFILE_DELETED_FL    0x04000000  /* Snapshot is being deleted */
#define EXT4_SNAPFILE_SHRUNK_FL        0x08000000  /* Snapshot shrink has completed */
#define EXT4_INLINE_DATA_FL        0x10000000 /* Inode has inline data */
#define EXT4_PROJINHERIT_FL        0x20000000 /* Create with parents projid */
#define EXT4_CASEFOLD_FL        0x40000000 /* Casefolded file */
#define EXT2_RESERVED_FL        0x80000000 /* reserved for ext2 lib */

#define EXT2_FL_USER_VISIBLE        0x604BDFFF /* User visible flags */
#define EXT2_FL_USER_MODIFIABLE        0x604B80FF /* User modifiable flags */

/*
 * ioctl commands
 */

/* Used for online resize */
struct ext2_new_group_input {
    __u32 group;        /* Group number for this data */
    __u32 block_bitmap;    /* Absolute block number of block bitmap */
    __u32 inode_bitmap;    /* Absolute block number of inode bitmap */
    __u32 inode_table;    /* Absolute block number of inode table start */
    __u32 blocks_count;    /* Total number of blocks in this group */
    __u16 reserved_blocks;    /* Number of reserved blocks in this group */
    __u16 unused;        /* Number of reserved GDT blocks in group */
};

struct ext4_new_group_input {
    __u32 group;        /* Group number for this data */
    __u64 block_bitmap;    /* Absolute block number of block bitmap */
    __u64 inode_bitmap;    /* Absolute block number of inode bitmap */
    __u64 inode_table;    /* Absolute block number of inode table start */
    __u32 blocks_count;    /* Total number of blocks in this group */
    __u16 reserved_blocks;    /* Number of reserved blocks in this group */
    __u16 unused;
};

#ifdef __GNU__            /* Needed for the Hurd */
#define _IOT_ext2_new_group_input _IOT (_IOTS(__u32), 5, _IOTS(__u16), 2, 0, 0)
#endif

#define EXT2_IOC_GETFLAGS        _IOR('f', 1, long)
#define EXT2_IOC_SETFLAGS        _IOW('f', 2, long)
#define EXT2_IOC_GETVERSION        _IOR('v', 1, long)
#define EXT2_IOC_SETVERSION        _IOW('v', 2, long)
#define EXT2_IOC_GETVERSION_NEW        _IOR('f', 3, long)
#define EXT2_IOC_SETVERSION_NEW        _IOW('f', 4, long)
#define EXT2_IOC_GROUP_EXTEND        _IOW('f', 7, unsigned long)
#define EXT2_IOC_GROUP_ADD        _IOW('f', 8,struct ext2_new_group_input)
#define EXT4_IOC_GROUP_ADD        _IOW('f', 8,struct ext4_new_group_input)
#define EXT4_IOC_RESIZE_FS        _IOW('f', 16, __u64)

/*
 * Structure of an inode on the disk
 */
struct ext2_inode {
/*00*/    __u16    i_mode;        /* File mode */
    __u16    i_uid;        /* Low 16 bits of Owner Uid */
    __u32    i_size;        /* Size in bytes */
    __u32    i_atime;    /* Access time */
    __u32    i_ctime;    /* Inode change time */
/*10*/    __u32    i_mtime;    /* Modification time */
    __u32    i_dtime;    /* Deletion Time */
    __u16    i_gid;        /* Low 16 bits of Group Id */
    __u16    i_links_count;    /* Links count */
    __u32    i_blocks;    /* Blocks count */
/*20*/    __u32    i_flags;    /* File flags */
    union {
        struct {
            __u32    l_i_version; /* was l_i_reserved1 */
        } linux1;
        struct {
            __u32  h_i_translator;
        } hurd1;
    } osd1;                /* OS dependent 1 */
/*28*/    __u32    i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
/*64*/    __u32    i_generation;    /* File version (for NFS) */
    __u32    i_file_acl;    /* File ACL */
    __u32    i_size_high;
/*70*/    __u32    i_faddr;    /* Fragment address */
    union {
        struct {
            __u16    l_i_blocks_hi;
            __u16    l_i_file_acl_high;
            __u16    l_i_uid_high;    /* these 2 fields    */
            __u16    l_i_gid_high;    /* were reserved2[0] */
            __u16    l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
            __u16    l_i_reserved;
        } linux2;
        struct {
            __u8    h_i_frag;    /* Fragment number */
            __u8    h_i_fsize;    /* Fragment size */
            __u16    h_i_mode_high;
            __u16    h_i_uid_high;
            __u16    h_i_gid_high;
            __u32    h_i_author;
        } hurd2;
    } osd2;                /* OS dependent 2 */
};

/*
 * Permanent part of an large inode on the disk
 */
struct ext2_inode_large {
/*00*/    __u16    i_mode;        /* File mode */
    __u16    i_uid;        /* Low 16 bits of Owner Uid */
    __u32    i_size;        /* Size in bytes */
    __u32    i_atime;    /* Access time */
    __u32    i_ctime;    /* Inode Change time */
/*10*/    __u32    i_mtime;    /* Modification time */
    __u32    i_dtime;    /* Deletion Time */
    __u16    i_gid;        /* Low 16 bits of Group Id */
    __u16    i_links_count;    /* Links count */
    __u32    i_blocks;    /* Blocks count */
/*20*/    __u32    i_flags;    /* File flags */
    union {
        struct {
            __u32    l_i_version; /* was l_i_reserved1 */
        } linux1;
        struct {
            __u32  h_i_translator;
        } hurd1;
    } osd1;                /* OS dependent 1 */
/*28*/    __u32    i_block[EXT2_N_BLOCKS];/* Pointers to blocks */
/*64*/    __u32    i_generation;    /* File version (for NFS) */
    __u32    i_file_acl;    /* File ACL */
    __u32    i_size_high;
/*70*/    __u32    i_faddr;    /* Fragment address */
    union {
        struct {
            __u16    l_i_blocks_hi;
            __u16    l_i_file_acl_high;
            __u16    l_i_uid_high;    /* these 2 fields    */
            __u16    l_i_gid_high;    /* were reserved2[0] */
            __u16    l_i_checksum_lo; /* crc32c(uuid+inum+inode) */
            __u16    l_i_reserved;
        } linux2;
        struct {
            __u8    h_i_frag;    /* Fragment number */
            __u8    h_i_fsize;    /* Fragment size */
            __u16    h_i_mode_high;
            __u16    h_i_uid_high;
            __u16    h_i_gid_high;
            __u32    h_i_author;
        } hurd2;
    } osd2;                /* OS dependent 2 */
/*80*/    __u16    i_extra_isize;
    __u16    i_checksum_hi;    /* crc32c(uuid+inum+inode) */
    __u32    i_ctime_extra;    /* extra Change time (nsec << 2 | epoch) */
    __u32    i_mtime_extra;    /* extra Modification time (nsec << 2 | epoch) */
    __u32    i_atime_extra;    /* extra Access time (nsec << 2 | epoch) */
/*90*/    __u32    i_crtime;    /* File creation time */
    __u32    i_crtime_extra;    /* extra File creation time (nsec << 2 | epoch)*/
    __u32    i_version_hi;    /* high 32 bits for 64-bit version */
/*9c*/    __u32   i_projid;       /* Project ID */
};

#define EXT4_INODE_CSUM_HI_EXTRA_END    \
    (offsetof(struct ext2_inode_large, i_checksum_hi) + sizeof(__u16) - \
     EXT2_GOOD_OLD_INODE_SIZE)

#define EXT4_EPOCH_BITS 2
#define EXT4_EPOCH_MASK ((1 << EXT4_EPOCH_BITS) - 1)

#define i_checksum_lo    osd2.linux2.l_i_checksum_lo

#define inode_includes(size, field)            \
       (size >= (sizeof(((struct ext2_inode_large *)0)->field) + \
                 offsetof(struct ext2_inode_large, field)))

#if defined(__KERNEL__) || defined(__linux__)
#define i_reserved1    osd1.linux1.l_i_reserved1
#define i_frag        osd2.linux2.l_i_frag
#define i_fsize        osd2.linux2.l_i_fsize
#define i_uid_low    i_uid
#define i_gid_low    i_gid
#define i_uid_high    osd2.linux2.l_i_uid_high
#define i_gid_high    osd2.linux2.l_i_gid_high
#else
#if defined(__GNU__)

#define i_translator    osd1.hurd1.h_i_translator
#define i_frag        osd2.hurd2.h_i_frag;
#define i_fsize        osd2.hurd2.h_i_fsize;
#define i_uid_high    osd2.hurd2.h_i_uid_high
#define i_gid_high    osd2.hurd2.h_i_gid_high
#define i_author    osd2.hurd2.h_i_author

#endif  /* __GNU__ */
#endif    /* defined(__KERNEL__) || defined(__linux__) */

#define inode_uid(inode)    ((inode).i_uid | (unsigned)(inode).osd2.linux2.l_i_uid_high << 16)
#define inode_gid(inode)    ((inode).i_gid | (unsigned)(inode).osd2.linux2.l_i_gid_high << 16)
#define inode_projid(inode)    ((inode).i_projid)
#define ext2fs_set_i_uid_high(inode,x) ((inode).osd2.linux2.l_i_uid_high = (x))
#define ext2fs_set_i_gid_high(inode,x) ((inode).osd2.linux2.l_i_gid_high = (x))

static inline
struct ext2_inode *EXT2_INODE(struct ext2_inode_large *large_inode)
{
    return (struct ext2_inode *) large_inode;
}

/*
 * File system states
 */
#define EXT2_VALID_FS            0x0001    /* Unmounted cleanly */
#define EXT2_ERROR_FS            0x0002    /* Errors detected */
#define EXT3_ORPHAN_FS            0x0004    /* Orphans being recovered */
#define EXT4_FC_REPLAY            0x0020    /* Ext4 fast commit replay ongoing */

/*
 * Misc. filesystem flags
 */
#define EXT2_FLAGS_SIGNED_HASH        0x0001  /* Signed dirhash in use */
#define EXT2_FLAGS_UNSIGNED_HASH    0x0002  /* Unsigned dirhash in use */
#define EXT2_FLAGS_TEST_FILESYS        0x0004    /* OK for use on development code */
#define EXT2_FLAGS_IS_SNAPSHOT        0x0010    /* This is a snapshot image */
#define EXT2_FLAGS_FIX_SNAPSHOT        0x0020    /* Snapshot inodes corrupted */
#define EXT2_FLAGS_FIX_EXCLUDE        0x0040    /* Exclude bitmaps corrupted */

/*
 * Mount flags
 */
#define EXT2_MOUNT_CHECK        0x0001    /* Do mount-time checks */
#define EXT2_MOUNT_GRPID        0x0004    /* Create files with directory's group */
#define EXT2_MOUNT_DEBUG        0x0008    /* Some debugging messages */
#define EXT2_MOUNT_ERRORS_CONT        0x0010    /* Continue on errors */
#define EXT2_MOUNT_ERRORS_RO        0x0020    /* Remount fs ro on errors */
#define EXT2_MOUNT_ERRORS_PANIC        0x0040    /* Panic on errors */
#define EXT2_MOUNT_MINIX_DF        0x0080    /* Mimics the Minix statfs */
#define EXT2_MOUNT_NO_UID32        0x0200  /* Disable 32-bit UIDs */

#define clear_opt(o, opt)        o &= ~EXT2_MOUNT_##opt
#define set_opt(o, opt)            o |= EXT2_MOUNT_##opt
#define test_opt(sb, opt)        (EXT2_SB(sb)->s_mount_opt & \
                     EXT2_MOUNT_##opt)
/*
 * Maximal mount counts between two filesystem checks
 */
#define EXT2_DFL_MAX_MNT_COUNT        20    /* Allow 20 mounts */
#define EXT2_DFL_CHECKINTERVAL        0    /* Don't use interval check */

/*
 * Behaviour when detecting errors
 */
#define EXT2_ERRORS_CONTINUE        1    /* Continue execution */
#define EXT2_ERRORS_RO            2    /* Remount fs read-only */
#define EXT2_ERRORS_PANIC        3    /* Panic */
#define EXT2_ERRORS_DEFAULT        EXT2_ERRORS_CONTINUE

#if (__GNUC__ >= 4)
#define ext4_offsetof(TYPE,MEMBER) __builtin_offsetof(TYPE,MEMBER)
#else
#define ext4_offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)
#endif

/* Metadata checksum algorithms */
#define EXT2_CRC32C_CHKSUM        1

/* Encryption algorithms, key size and key reference len */
#define EXT4_ENCRYPTION_MODE_INVALID        0
#define EXT4_ENCRYPTION_MODE_AES_256_XTS    1
#define EXT4_ENCRYPTION_MODE_AES_256_GCM    2
#define EXT4_ENCRYPTION_MODE_AES_256_CBC    3
#define EXT4_ENCRYPTION_MODE_AES_256_CTS    4

#define EXT4_AES_256_XTS_KEY_SIZE        64
#define EXT4_AES_256_GCM_KEY_SIZE        32
#define EXT4_AES_256_CBC_KEY_SIZE        32
#define EXT4_AES_256_CTS_KEY_SIZE        32
#define EXT4_MAX_KEY_SIZE            64

#define EXT4_KEY_DESCRIPTOR_SIZE        8
#define EXT4_CRYPTO_BLOCK_SIZE            16

/* Password derivation constants */
#define EXT4_MAX_PASSPHRASE_SIZE        1024
#define EXT4_MAX_SALT_SIZE            256
#define EXT4_PBKDF2_ITERATIONS            0xFFFF

#define EXT2_LABEL_LEN                16

/*
 * Policy provided via an ioctl on the topmost directory. This
 * structure is also in the kernel.
 */
struct ext4_encryption_policy {
  char version;
  char contents_encryption_mode;
  char filenames_encryption_mode;
  char flags;
  char master_key_descriptor[EXT4_KEY_DESCRIPTOR_SIZE];
} __attribute__((__packed__));

struct ext4_encryption_key {
        __u32 mode;
        char raw[EXT4_MAX_KEY_SIZE];
        __u32 size;
} __attribute__((__packed__));

/*
 * Structure of the super block
 */
struct ext2_super_block {
/*000*/    __u32    s_inodes_count;        /* Inodes count */
    __u32    s_blocks_count;        /* Blocks count */
    __u32    s_r_blocks_count;    /* Reserved blocks count */
    __u32    s_free_blocks_count;    /* Free blocks count */
/*010*/    __u32    s_free_inodes_count;    /* Free inodes count */
    __u32    s_first_data_block;    /* First Data Block */
    __u32    s_log_block_size;    /* Block size */
    __u32    s_log_cluster_size;    /* Allocation cluster size */
/*020*/    __u32    s_blocks_per_group;    /* # Blocks per group */
    __u32    s_clusters_per_group;    /* # Fragments per group */
    __u32    s_inodes_per_group;    /* # Inodes per group */
    __u32    s_mtime;        /* Mount time */
/*030*/    __u32    s_wtime;        /* Write time */
    __u16    s_mnt_count;        /* Mount count */
    __s16    s_max_mnt_count;    /* Maximal mount count */
    __u16    s_magic;        /* Magic signature */
    __u16    s_state;        /* File system state */
    __u16    s_errors;        /* Behaviour when detecting errors */
    __u16    s_minor_rev_level;    /* minor revision level */
/*040*/    __u32    s_lastcheck;        /* time of last check */
    __u32    s_checkinterval;    /* max. time between checks */
    __u32    s_creator_os;        /* OS */
    __u32    s_rev_level;        /* Revision level */
/*050*/    __u16    s_def_resuid;        /* Default uid for reserved blocks */
    __u16    s_def_resgid;        /* Default gid for reserved blocks */
    /*
     * These fields are for EXT2_DYNAMIC_REV superblocks only.
     *
     * Note: the difference between the compatible feature set and
     * the incompatible feature set is that if there is a bit set
     * in the incompatible feature set that the kernel doesn't
     * know about, it should refuse to mount the filesystem.
     *
     * e2fsck's requirements are more strict; if it doesn't know
     * about a feature in either the compatible or incompatible
     * feature set, it must abort and not try to meddle with
     * things it doesn't understand...
     */
    __u32    s_first_ino;        /* First non-reserved inode */
    __u16   s_inode_size;        /* size of inode structure */
    __u16    s_block_group_nr;    /* block group # of this superblock */
    __u32    s_feature_compat;    /* compatible feature set */
/*060*/    __u32    s_feature_incompat;    /* incompatible feature set */
    __u32    s_feature_ro_compat;    /* readonly-compatible feature set */
/*068*/    __u8    s_uuid[16] __nonstring;        /* 128-bit uuid for volume */
/*078*/    __u8    s_volume_name[EXT2_LABEL_LEN] __nonstring;    /* volume name, no NUL? */
/*088*/    __u8    s_last_mounted[64] __nonstring;    /* directory last mounted on, no NUL? */
/*0c8*/    __u32    s_algorithm_usage_bitmap; /* For compression */
    /*
     * Performance hints.  Directory preallocation should only
     * happen if the EXT2_FEATURE_COMPAT_DIR_PREALLOC flag is on.
     */
    __u8    s_prealloc_blocks;    /* Nr of blocks to try to preallocate*/
    __u8    s_prealloc_dir_blocks;    /* Nr to preallocate for dirs */
    __u16    s_reserved_gdt_blocks;    /* Per group table for online growth */
    /*
     * Journaling support valid if EXT2_FEATURE_COMPAT_HAS_JOURNAL set.
     */
/*0d0*/    __u8    s_journal_uuid[16] __nonstring;    /* uuid of journal superblock */
/*0e0*/    __u32    s_journal_inum;        /* inode number of journal file */
    __u32    s_journal_dev;        /* device number of journal file */
    __u32    s_last_orphan;        /* start of list of inodes to delete */
/*0ec*/    __u32    s_hash_seed[4];        /* HTREE hash seed */
/*0fc*/    __u8    s_def_hash_version;    /* Default hash version to use */
    __u8    s_jnl_backup_type;    /* Default type of journal backup */
    __u16    s_desc_size;        /* Group desc. size: INCOMPAT_64BIT */
/*100*/    __u32    s_default_mount_opts;    /* default EXT2_MOUNT_* flags used */
    __u32    s_first_meta_bg;    /* First metablock group */
    __u32    s_mkfs_time;        /* When the filesystem was created */
/*10c*/    __u32    s_jnl_blocks[17];    /* Backup of the journal inode */
/*150*/    __u32    s_blocks_count_hi;    /* Blocks count high 32bits */
    __u32    s_r_blocks_count_hi;    /* Reserved blocks count high 32 bits*/
    __u32    s_free_blocks_hi;    /* Free blocks count */
    __u16    s_min_extra_isize;    /* All inodes have at least # bytes */
    __u16    s_want_extra_isize;    /* New inodes should reserve # bytes */
/*160*/    __u32    s_flags;        /* Miscellaneous flags */
    __u16    s_raid_stride;        /* RAID stride in blocks */
    __u16    s_mmp_update_interval;  /* # seconds to wait in MMP checking */
    __u64    s_mmp_block;        /* Block for multi-mount protection */
/*170*/    __u32    s_raid_stripe_width;    /* blocks on all data disks (N*stride)*/
    __u8    s_log_groups_per_flex;    /* FLEX_BG group size */
    __u8    s_checksum_type;    /* metadata checksum algorithm */
    __u8    s_encryption_level;    /* versioning level for encryption */
    __u8    s_reserved_pad;        /* Padding to next 32bits */
    __u64    s_kbytes_written;    /* nr of lifetime kilobytes written */
/*180*/    __u32    s_snapshot_inum;    /* Inode number of active snapshot */
    __u32    s_snapshot_id;        /* sequential ID of active snapshot */
    __u64    s_snapshot_r_blocks_count; /* active snapshot reserved blocks */
/*190*/    __u32    s_snapshot_list;    /* inode number of disk snapshot list */
#define EXT4_S_ERR_START ext4_offsetof(struct ext2_super_block, s_error_count)
    __u32    s_error_count;        /* number of fs errors */
    __u32    s_first_error_time;    /* first time an error happened */
    __u32    s_first_error_ino;    /* inode involved in first error */
/*1a0*/    __u64    s_first_error_block;    /* block involved in first error */
    __u8    s_first_error_func[32] __nonstring;    /* function where error hit, no NUL? */
/*1c8*/    __u32    s_first_error_line;    /* line number where error happened */
    __u32    s_last_error_time;    /* most recent time of an error */
/*1d0*/    __u32    s_last_error_ino;    /* inode involved in last error */
    __u32    s_last_error_line;    /* line number where error happened */
    __u64    s_last_error_block;    /* block involved of last error */
/*1e0*/    __u8    s_last_error_func[32] __nonstring;    /* function where error hit, no NUL? */
#define EXT4_S_ERR_END ext4_offsetof(struct ext2_super_block, s_mount_opts)
/*200*/    __u8    s_mount_opts[64] __nonstring;    /* default mount options, no NUL? */
/*240*/    __u32    s_usr_quota_inum;    /* inode number of user quota file */
    __u32    s_grp_quota_inum;    /* inode number of group quota file */
    __u32    s_overhead_clusters;    /* overhead blocks/clusters in fs */
/*24c*/    __u32    s_backup_bgs[2];    /* If sparse_super2 enabled */
/*254*/    __u8    s_encrypt_algos[4];    /* Encryption algorithms in use  */
/*258*/    __u8    s_encrypt_pw_salt[16];    /* Salt used for string2key algorithm */
/*268*/    __le32    s_lpf_ino;        /* Location of the lost+found inode */
    __le32  s_prj_quota_inum;    /* inode for tracking project quota */
/*270*/    __le32    s_checksum_seed;    /* crc32c(orig_uuid) if csum_seed set */
/*274*/    __u8    s_wtime_hi;
    __u8    s_mtime_hi;
    __u8    s_mkfs_time_hi;
    __u8    s_lastcheck_hi;
    __u8    s_first_error_time_hi;
    __u8    s_last_error_time_hi;
    __u8    s_first_error_errcode;
    __u8    s_last_error_errcode;
/*27c*/ __le16    s_encoding;        /* Filename charset encoding */
    __le16    s_encoding_flags;    /* Filename charset encoding flags */
    __le32  s_orphan_file_inum;    /* Inode for tracking orphan inodes */
    __le32    s_reserved[94];        /* Padding to the end of the block */
/*3fc*/    __u32    s_checksum;        /* crc32c(superblock) */
};

#define EXT4_S_ERR_LEN (EXT4_S_ERR_END - EXT4_S_ERR_START)
#define EXT2_LEN_STR(buf) (int)sizeof(buf), (char *)buf

/*
 * Codes for operating systems
 */
#define EXT2_OS_LINUX        0
#define EXT2_OS_HURD        1
#define EXT2_OBSO_OS_MASIX    2
#define EXT2_OS_FREEBSD        3
#define EXT2_OS_LITES        4

/*
 * Revision levels
 */
#define EXT2_GOOD_OLD_REV    0    /* The good old (original) format */
#define EXT2_DYNAMIC_REV    1    /* V2 format w/ dynamic inode sizes */

#define EXT2_CURRENT_REV    EXT2_GOOD_OLD_REV
#define EXT2_MAX_SUPP_REV    EXT2_DYNAMIC_REV

#define EXT2_GOOD_OLD_INODE_SIZE 128

/*
 * Journal inode backup types
 */
#define EXT3_JNL_BACKUP_BLOCKS    1

/*
 * Feature set definitions
 */

#define EXT2_HAS_COMPAT_FEATURE(sb,mask)            \
    ( EXT2_SB(sb)->s_feature_compat & (mask) )
#define EXT2_HAS_RO_COMPAT_FEATURE(sb,mask)            \
    ( EXT2_SB(sb)->s_feature_ro_compat & (mask) )
#define EXT2_HAS_INCOMPAT_FEATURE(sb,mask)            \
    ( EXT2_SB(sb)->s_feature_incompat & (mask) )

#define EXT2_FEATURE_COMPAT_DIR_PREALLOC    0x0001
#define EXT2_FEATURE_COMPAT_IMAGIC_INODES    0x0002
#define EXT3_FEATURE_COMPAT_HAS_JOURNAL        0x0004
#define EXT2_FEATURE_COMPAT_EXT_ATTR        0x0008
#define EXT2_FEATURE_COMPAT_RESIZE_INODE    0x0010
#define EXT2_FEATURE_COMPAT_DIR_INDEX        0x0020
#define EXT2_FEATURE_COMPAT_LAZY_BG        0x0040
/* #define EXT2_FEATURE_COMPAT_EXCLUDE_INODE    0x0080 not used, legacy */
#define EXT2_FEATURE_COMPAT_EXCLUDE_BITMAP    0x0100
#define EXT4_FEATURE_COMPAT_SPARSE_SUPER2    0x0200
#define EXT4_FEATURE_COMPAT_FAST_COMMIT        0x0400
#define EXT4_FEATURE_COMPAT_STABLE_INODES    0x0800
#define EXT4_FEATURE_COMPAT_ORPHAN_FILE        0x1000

#define EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER    0x0001
#define EXT2_FEATURE_RO_COMPAT_LARGE_FILE    0x0002
/* #define EXT2_FEATURE_RO_COMPAT_BTREE_DIR    0x0004 not used */
#define EXT4_FEATURE_RO_COMPAT_HUGE_FILE    0x0008
#define EXT4_FEATURE_RO_COMPAT_GDT_CSUM        0x0010
#define EXT4_FEATURE_RO_COMPAT_DIR_NLINK    0x0020
#define EXT4_FEATURE_RO_COMPAT_EXTRA_ISIZE    0x0040
#define EXT4_FEATURE_RO_COMPAT_HAS_SNAPSHOT    0x0080
#define EXT4_FEATURE_RO_COMPAT_QUOTA        0x0100
#define EXT4_FEATURE_RO_COMPAT_BIGALLOC        0x0200
/*
 * METADATA_CSUM implies GDT_CSUM.  When METADATA_CSUM is set, group
 * descriptor checksums use the same algorithm as all other data
 * structures' checksums.
 */
#define EXT4_FEATURE_RO_COMPAT_METADATA_CSUM    0x0400
#define EXT4_FEATURE_RO_COMPAT_REPLICA        0x0800
#define EXT4_FEATURE_RO_COMPAT_READONLY        0x1000
#define EXT4_FEATURE_RO_COMPAT_PROJECT        0x2000 /* Project quota */
#define EXT4_FEATURE_RO_COMPAT_SHARED_BLOCKS    0x4000
#define EXT4_FEATURE_RO_COMPAT_VERITY        0x8000
#define EXT4_FEATURE_RO_COMPAT_ORPHAN_PRESENT    0x10000

#define EXT2_FEATURE_INCOMPAT_COMPRESSION    0x0001
#define EXT2_FEATURE_INCOMPAT_FILETYPE        0x0002
#define EXT3_FEATURE_INCOMPAT_RECOVER        0x0004 /* Needs recovery */
#define EXT3_FEATURE_INCOMPAT_JOURNAL_DEV    0x0008 /* Journal device */
#define EXT2_FEATURE_INCOMPAT_META_BG        0x0010
#define EXT3_FEATURE_INCOMPAT_EXTENTS        0x0040
#define EXT4_FEATURE_INCOMPAT_64BIT        0x0080
#define EXT4_FEATURE_INCOMPAT_MMP        0x0100
#define EXT4_FEATURE_INCOMPAT_FLEX_BG        0x0200
#define EXT4_FEATURE_INCOMPAT_EA_INODE        0x0400
#define EXT4_FEATURE_INCOMPAT_DIRDATA        0x1000
#define EXT4_FEATURE_INCOMPAT_CSUM_SEED        0x2000
#define EXT4_FEATURE_INCOMPAT_LARGEDIR        0x4000 /* >2GB or 3-lvl htree */
#define EXT4_FEATURE_INCOMPAT_INLINE_DATA    0x8000 /* data in inode */
#define EXT4_FEATURE_INCOMPAT_ENCRYPT        0x10000
#define EXT4_FEATURE_INCOMPAT_CASEFOLD        0x20000

#define EXT4_FEATURE_COMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
    return ((EXT2_SB(sb)->s_feature_compat & \
         EXT##ver##_FEATURE_COMPAT_##flagname) != 0); \
} \
static inline void ext2fs_set_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_compat |= \
        EXT##ver##_FEATURE_COMPAT_##flagname; \
} \
static inline void ext2fs_clear_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_compat &= \
        ~EXT##ver##_FEATURE_COMPAT_##flagname; \
}

#define EXT4_FEATURE_RO_COMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
    return ((EXT2_SB(sb)->s_feature_ro_compat & \
         EXT##ver##_FEATURE_RO_COMPAT_##flagname) != 0); \
} \
static inline void ext2fs_set_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_ro_compat |= \
        EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
} \
static inline void ext2fs_clear_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_ro_compat &= \
        ~EXT##ver##_FEATURE_RO_COMPAT_##flagname; \
}

#define EXT4_FEATURE_INCOMPAT_FUNCS(name, ver, flagname) \
static inline int ext2fs_has_feature_##name(struct ext2_super_block *sb) \
{ \
    return ((EXT2_SB(sb)->s_feature_incompat & \
         EXT##ver##_FEATURE_INCOMPAT_##flagname) != 0); \
} \
static inline void ext2fs_set_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_incompat |= \
        EXT##ver##_FEATURE_INCOMPAT_##flagname; \
} \
static inline void ext2fs_clear_feature_##name(struct ext2_super_block *sb) \
{ \
    EXT2_SB(sb)->s_feature_incompat &= \
        ~EXT##ver##_FEATURE_INCOMPAT_##flagname; \
}

EXT4_FEATURE_COMPAT_FUNCS(dir_prealloc,        2, DIR_PREALLOC)
EXT4_FEATURE_COMPAT_FUNCS(imagic_inodes,    2, IMAGIC_INODES)
EXT4_FEATURE_COMPAT_FUNCS(journal,        3, HAS_JOURNAL)
EXT4_FEATURE_COMPAT_FUNCS(xattr,        2, EXT_ATTR)
EXT4_FEATURE_COMPAT_FUNCS(resize_inode,        2, RESIZE_INODE)
EXT4_FEATURE_COMPAT_FUNCS(dir_index,        2, DIR_INDEX)
EXT4_FEATURE_COMPAT_FUNCS(lazy_bg,        2, LAZY_BG)
EXT4_FEATURE_COMPAT_FUNCS(exclude_bitmap,    2, EXCLUDE_BITMAP)
EXT4_FEATURE_COMPAT_FUNCS(sparse_super2,    4, SPARSE_SUPER2)
EXT4_FEATURE_COMPAT_FUNCS(fast_commit,        4, FAST_COMMIT)
EXT4_FEATURE_COMPAT_FUNCS(stable_inodes,    4, STABLE_INODES)
EXT4_FEATURE_COMPAT_FUNCS(orphan_file,        4, ORPHAN_FILE)

EXT4_FEATURE_RO_COMPAT_FUNCS(sparse_super,    2, SPARSE_SUPER)
EXT4_FEATURE_RO_COMPAT_FUNCS(large_file,    2, LARGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(huge_file,        4, HUGE_FILE)
EXT4_FEATURE_RO_COMPAT_FUNCS(gdt_csum,        4, GDT_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(dir_nlink,        4, DIR_NLINK)
EXT4_FEATURE_RO_COMPAT_FUNCS(extra_isize,    4, EXTRA_ISIZE)
EXT4_FEATURE_RO_COMPAT_FUNCS(has_snapshot,    4, HAS_SNAPSHOT)
EXT4_FEATURE_RO_COMPAT_FUNCS(quota,        4, QUOTA)
EXT4_FEATURE_RO_COMPAT_FUNCS(bigalloc,        4, BIGALLOC)
EXT4_FEATURE_RO_COMPAT_FUNCS(metadata_csum,    4, METADATA_CSUM)
EXT4_FEATURE_RO_COMPAT_FUNCS(replica,        4, REPLICA)
EXT4_FEATURE_RO_COMPAT_FUNCS(readonly,        4, READONLY)
EXT4_FEATURE_RO_COMPAT_FUNCS(project,        4, PROJECT)
EXT4_FEATURE_RO_COMPAT_FUNCS(shared_blocks,    4, SHARED_BLOCKS)
EXT4_FEATURE_RO_COMPAT_FUNCS(verity,        4, VERITY)
EXT4_FEATURE_RO_COMPAT_FUNCS(orphan_present,    4, ORPHAN_PRESENT)

EXT4_FEATURE_INCOMPAT_FUNCS(compression,    2, COMPRESSION)
EXT4_FEATURE_INCOMPAT_FUNCS(filetype,        2, FILETYPE)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_needs_recovery,    3, RECOVER)
EXT4_FEATURE_INCOMPAT_FUNCS(journal_dev,    3, JOURNAL_DEV)
EXT4_FEATURE_INCOMPAT_FUNCS(meta_bg,        2, META_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(extents,        3, EXTENTS)
EXT4_FEATURE_INCOMPAT_FUNCS(64bit,        4, 64BIT)
EXT4_FEATURE_INCOMPAT_FUNCS(mmp,        4, MMP)
EXT4_FEATURE_INCOMPAT_FUNCS(flex_bg,        4, FLEX_BG)
EXT4_FEATURE_INCOMPAT_FUNCS(ea_inode,        4, EA_INODE)
EXT4_FEATURE_INCOMPAT_FUNCS(dirdata,        4, DIRDATA)
EXT4_FEATURE_INCOMPAT_FUNCS(csum_seed,        4, CSUM_SEED)
EXT4_FEATURE_INCOMPAT_FUNCS(largedir,        4, LARGEDIR)
EXT4_FEATURE_INCOMPAT_FUNCS(inline_data,    4, INLINE_DATA)
EXT4_FEATURE_INCOMPAT_FUNCS(encrypt,        4, ENCRYPT)
EXT4_FEATURE_INCOMPAT_FUNCS(casefold,        4, CASEFOLD)

#define EXT2_FEATURE_COMPAT_SUPP    0
#define EXT2_FEATURE_INCOMPAT_SUPP    (EXT2_FEATURE_INCOMPAT_FILETYPE| \
                       EXT4_FEATURE_INCOMPAT_MMP| \
                       EXT4_FEATURE_INCOMPAT_LARGEDIR| \
                       EXT4_FEATURE_INCOMPAT_EA_INODE)
#define EXT2_FEATURE_RO_COMPAT_SUPP    (EXT2_FEATURE_RO_COMPAT_SPARSE_SUPER| \
                     EXT2_FEATURE_RO_COMPAT_LARGE_FILE| \
                     EXT4_FEATURE_RO_COMPAT_DIR_NLINK| \
                     EXT2_FEATURE_RO_COMPAT_BTREE_DIR| \
                     EXT4_FEATURE_RO_COMPAT_VERITY)

/*
 * Default values for user and/or group using reserved blocks
 */
#define EXT2_DEF_RESUID        0
#define EXT2_DEF_RESGID        0

/*
 * Default mount options
 */
#define EXT2_DEFM_DEBUG        0x0001
#define EXT2_DEFM_BSDGROUPS    0x0002
#define EXT2_DEFM_XATTR_USER    0x0004
#define EXT2_DEFM_ACL        0x0008
#define EXT2_DEFM_UID16        0x0010
#define EXT3_DEFM_JMODE        0x0060
#define EXT3_DEFM_JMODE_DATA    0x0020
#define EXT3_DEFM_JMODE_ORDERED    0x0040
#define EXT3_DEFM_JMODE_WBACK    0x0060
#define EXT4_DEFM_NOBARRIER    0x0100
#define EXT4_DEFM_BLOCK_VALIDITY 0x0200
#define EXT4_DEFM_DISCARD    0x0400
#define EXT4_DEFM_NODELALLOC    0x0800

static inline int ext4_hash_in_dirent(const struct ext2_inode *inode)
{
    return (inode->i_flags & EXT4_ENCRYPT_FL) &&
        (inode->i_flags & EXT4_CASEFOLD_FL);
}

/*
 * Structure of a directory entry
 */
#define EXT2_NAME_LEN 255

struct ext2_dir_entry {
    __u32    inode;            /* Inode number */
    __u16    rec_len;        /* Directory entry length */
    __u16    name_len;        /* Name length */
    char    name[EXT2_NAME_LEN];    /* File name */
};

/*
 * The new version of the directory entry.  Since EXT2 structures are
 * stored in intel byte order, and the name_len field could never be
 * bigger than 255 chars, it's safe to reclaim the extra byte for the
 * file_type field.
 *
 * This structure is deprecated due to endian issues. Please use struct
 * ext2_dir_entry and accessor functions
 *   ext2fs_dirent_name_len
 *   ext2fs_dirent_set_name_len
 *   ext2fs_dirent_file_type
 *   ext2fs_dirent_set_file_type
 * to get and set name_len and file_type fields.
 */
struct ext2_dir_entry_2 {
    __u32    inode;            /* Inode number */
    __u16    rec_len;        /* Directory entry length */
    __u8    name_len;        /* Name length */
    __u8    file_type;
    char    name[EXT2_NAME_LEN];    /* File name */
};

/*
 * Hashes for ext4_dir_entry for casefolded and ecrypted directories.
 * This is located at the first 4 bit aligned location after the name.
 */

struct ext2_dir_entry_hash {
    __le32 hash;
    __le32 minor_hash;
};

#define EXT2_DIRENT_HASHES(entry) \
    ((struct ext2_dir_entry_hash *) &entry->name[\
        (ext2fs_dirent_name_len(entry) + \
            EXT2_DIR_ROUND) & ~EXT2_DIR_ROUND])
#define EXT2_DIRENT_HASH(entry) \
        ext2fs_le32_to_cpu(EXT2_DIRENT_HASHES(entry)->hash)
#define EXT2_DIRENT_MINOR_HASH(entry) \
        ext2fs_le32_to_cpu(EXT2_DIRENT_HASHES(entry)->minor_hash)

/*
 * This is a bogus directory entry at the end of each leaf block that
 * records checksums.
 */
struct ext2_dir_entry_tail {
    __u32    det_reserved_zero1;    /* Pretend to be unused */
    __u16    det_rec_len;        /* 12 */
    __u16    det_reserved_name_len;    /* 0xDE00, fake namelen/filetype */
    __u32    det_checksum;        /* crc32c(uuid+inode+dirent) */
};

/*
 * Ext2 directory file types.  Only the low 3 bits are used.  The
 * other bits are reserved for now.
 */
#define EXT2_FT_UNKNOWN        0
#define EXT2_FT_REG_FILE    1
#define EXT2_FT_DIR        2
#define EXT2_FT_CHRDEV        3
#define EXT2_FT_BLKDEV        4
#define EXT2_FT_FIFO        5
#define EXT2_FT_SOCK        6
#define EXT2_FT_SYMLINK        7

#define EXT2_FT_MAX        8

/*
 * Annoyingly, e2fsprogs always swab16s ext2_dir_entry.name_len, so we
 * have to build ext2_dir_entry_tail with that assumption too.  This
 * constant helps to build the dir_entry_tail to look like it has an
 * "invalid" file type.
 */
#define EXT2_DIR_NAME_LEN_CSUM    0xDE00

/*
 * EXT2_DIR_PAD defines the directory entries boundaries
 *
 * NOTE: It must be a multiple of 4
 */
#define EXT2_DIR_ENTRY_HEADER_LEN    8
#define EXT2_DIR_ENTRY_HASH_LEN        8
#define EXT2_DIR_PAD            4
#define EXT2_DIR_ROUND            (EXT2_DIR_PAD - 1)
#define EXT2_DIR_REC_LEN(name_len) ext2fs_dir_rec_len(name_len, 0)

static inline unsigned int ext2fs_dir_rec_len(__u8 name_len,
                        int extended)
{
    int rec_len = (name_len + EXT2_DIR_ENTRY_HEADER_LEN + EXT2_DIR_ROUND);

    rec_len &= ~EXT2_DIR_ROUND;
    if (extended)
        rec_len += EXT2_DIR_ENTRY_HASH_LEN;
    return rec_len;
}

#define EXT4_ORPHAN_BLOCK_MAGIC 0x0b10ca04

/* Structure at the tail of orphan block */
struct ext4_orphan_block_tail {
    __u32 ob_magic;
    __u32 ob_checksum;
};

/*
 * Constants for ext4's extended time encoding
 */
#define EXT4_EPOCH_BITS 2
#define EXT4_EPOCH_MASK ((1 << EXT4_EPOCH_BITS) - 1)
#define EXT4_NSEC_MASK  (~0UL << EXT4_EPOCH_BITS)

/*
 * This structure is used for multiple mount protection. It is written
 * into the block number saved in the s_mmp_block field in the superblock.
 * Programs that check MMP should assume that if SEQ_FSCK (or any unknown
 * code above SEQ_MAX) is present then it is NOT safe to use the filesystem,
 * regardless of how old the timestamp is.
 *
 * The timestamp in the MMP structure will be updated by e2fsck at some
 * arbitrary intervals (start of passes, after every few groups of inodes
 * in pass1 and pass1b).  There is no guarantee that e2fsck is updating
 * the MMP block in a timely manner, and the updates it does are purely
 * for the convenience of the sysadmin and not for automatic validation.
 *
 * Note: Only the mmp_seq value is used to determine whether the MMP block
 *    is being updated.  The mmp_time, mmp_nodename, and mmp_bdevname
 *    fields are only for informational purposes for the administrator,
 *    due to clock skew between nodes and hostname HA service takeover.
 */
#define EXT4_MMP_MAGIC     0x004D4D50U /* ASCII for MMP */
#define EXT4_MMP_SEQ_CLEAN 0xFF4D4D50U /* mmp_seq value for clean unmount */
#define EXT4_MMP_SEQ_FSCK  0xE24D4D50U /* mmp_seq value when being fscked */
#define EXT4_MMP_SEQ_MAX   0xE24D4D4FU /* maximum valid mmp_seq value */

/* Not endian-annotated; it's swapped at read/write time */
struct mmp_struct {
    __u32    mmp_magic;        /* Magic number for MMP */
    __u32    mmp_seq;        /* Sequence no. updated periodically */
    __u64    mmp_time;        /* Time last updated (seconds) */
    __u8    mmp_nodename[64] __nonstring;    /* Node updating MMP block, no NUL? */
    __u8    mmp_bdevname[32] __nonstring;    /* Bdev updating MMP block, no NUL? */
    __u16    mmp_check_interval;    /* Changed mmp_check_interval */
    __u16    mmp_pad1;
    __u32    mmp_pad2[226];
    __u32    mmp_checksum;        /* crc32c(uuid+mmp_block) */
};

/*
 * Default interval for MMP update in seconds.
 */
#define EXT4_MMP_UPDATE_INTERVAL    5

/*
 * Maximum interval for MMP update in seconds.
 */
#define EXT4_MMP_MAX_UPDATE_INTERVAL    300

/*
 * Minimum interval for MMP checking in seconds.
 */
#define EXT4_MMP_MIN_CHECK_INTERVAL     5

/*
 * Minimum size of inline data.
 */
#define EXT4_MIN_INLINE_DATA_SIZE    ((sizeof(__u32) * EXT2_N_BLOCKS))

/*
 * Size of a parent inode in inline data directory.
 */
#define EXT4_INLINE_DATA_DOTDOT_SIZE    (4)

#define EXT4_ENC_UTF8_12_1    1

#define EXT4_ENC_STRICT_MODE_FL            (1 << 0) /* Reject invalid sequences */

#endif    /* _LINUX_EXT2_FS_H */

/*
 * from e2fsprogs lib/ext2fs/ext2fs.h:
 */

/*
 * Where the master copy of the superblock is located, and how big
 * superblocks are supposed to be.  We define SUPERBLOCK_SIZE because
 * the size of the superblock structure is not necessarily trustworthy
 * (some versions have the padding set up so that the superblock is
 * 1032 bytes long).
 */
#define SUPERBLOCK_OFFSET   1024
#define SUPERBLOCK_SIZE     1024


/*
 * e2fsprogs lib/ext2fs/ext3_extents.h:
 */

#ifndef _LINUX_EXT3_EXTENTS
#define _LINUX_EXT3_EXTENTS

/*
 * ext3_inode has i_block array (total 60 bytes)
 * first 4 bytes are used to store:
 *  - tree depth (0 mean there is no tree yet. all extents in the inode)
 *  - number of alive extents in the inode
 */

/*
 * This is extent tail on-disk structure.
 * All other extent structures are 12 bytes long.  It turns out that
 * block_size % 12 >= 4 for at least all powers of 2 greater than 512, which
 * covers all valid ext4 block sizes.  Therefore, this tail structure can be
 * crammed into the end of the block without having to rebalance the tree.
 */
struct ext3_extent_tail {
    __le32    et_checksum;    /* crc32c(uuid+inum+extent_block) */
};

/*
 * this is extent on-disk structure
 * it's used at the bottom of the tree
 */
struct ext3_extent {
    __le32    ee_block;    /* first logical block extent covers */
    __le16    ee_len;        /* number of blocks covered by extent */
    __le16    ee_start_hi;    /* high 16 bits of physical block */
    __le32    ee_start;    /* low 32 bigs of physical block */
};

/*
 * this is index on-disk structure
 * it's used at all the levels, but the bottom
 */
struct ext3_extent_idx {
    __le32    ei_block;    /* index covers logical blocks from 'block' */
    __le32    ei_leaf;    /* pointer to the physical block of the next *
                 * level. leaf or next index could bet here */
    __le16    ei_leaf_hi;    /* high 16 bits of physical block */
    __le16    ei_unused;
};

/*
 * each block (leaves and indexes), even inode-stored has header
 */
struct ext3_extent_header {
    __le16    eh_magic;    /* probably will support different formats */
    __le16    eh_entries;    /* number of valid entries */
    __le16    eh_max;        /* capacity of store in entries */
    __le16    eh_depth;    /* has tree real underlying blocks? */
    __le32    eh_generation;    /* generation of the tree */
};

#define EXT3_EXT_MAGIC        0xf30a

/*
 * array of ext3_ext_path contains path to some extent
 * creation/lookup routines use it for traversal/splitting/etc
 * truncate uses it to simulate recursive walking
 */
struct ext3_ext_path {
    __u32                p_block;
    __u16                p_depth;
    struct ext3_extent        *p_ext;
    struct ext3_extent_idx        *p_idx;
    struct ext3_extent_header    *p_hdr;
    struct buffer_head        *p_bh;
};

/*
 * EXT_INIT_MAX_LEN is the maximum number of blocks we can have in an
 * initialized extent. This is 2^15 and not (2^16 - 1), since we use the
 * MSB of ee_len field in the extent datastructure to signify if this
 * particular extent is an initialized extent or an uninitialized (i.e.
 * preallocated).
 * EXT_UNINIT_MAX_LEN is the maximum number of blocks we can have in an
 * uninitialized extent.
 * If ee_len is <= 0x8000, it is an initialized extent. Otherwise, it is an
 * uninitialized one. In other words, if MSB of ee_len is set, it is an
 * uninitialized extent with only one special scenario when ee_len = 0x8000.
 * In this case we can not have an uninitialized extent of zero length and
 * thus we make it as a special case of initialized extent with 0x8000 length.
 * This way we get better extent-to-group alignment for initialized extents.
 * Hence, the maximum number of blocks we can have in an *initialized*
 * extent is 2^15 (32768) and in an *uninitialized* extent is 2^15-1 (32767).
 */
#define EXT_INIT_MAX_LEN    (1UL << 15)
#define EXT_UNINIT_MAX_LEN    (EXT_INIT_MAX_LEN - 1)
#define EXT_MAX_EXTENT_LBLK    (((__u64) 1 << 32) - 1)
#define EXT_MAX_EXTENT_PBLK    (((__u64) 1 << 48) - 1)

#define EXT_FIRST_EXTENT(__hdr__) \
    ((struct ext3_extent *) (((char *) (__hdr__)) +        \
                 sizeof(struct ext3_extent_header)))
#define EXT_FIRST_INDEX(__hdr__) \
    ((struct ext3_extent_idx *) (((char *) (__hdr__)) +    \
                     sizeof(struct ext3_extent_header)))
#define EXT_HAS_FREE_INDEX(__path__) \
    (ext2fs_le16_to_cpu((__path__)->p_hdr->eh_entries) < \
     ext2fs_le16_to_cpu((__path__)->p_hdr->eh_max))
#define EXT_LAST_EXTENT(__hdr__) \
    (EXT_FIRST_EXTENT((__hdr__)) + \
    ext2fs_le16_to_cpu((__hdr__)->eh_entries) - 1)
#define EXT_LAST_INDEX(__hdr__) \
    (EXT_FIRST_INDEX((__hdr__)) + \
    ext2fs_le16_to_cpu((__hdr__)->eh_entries) - 1)
#define EXT_MAX_EXTENT(__hdr__) \
    (EXT_FIRST_EXTENT((__hdr__)) + \
    ext2fs_le16_to_cpu((__hdr__)->eh_max) - 1)
#define EXT_MAX_INDEX(__hdr__) \
    (EXT_FIRST_INDEX((__hdr__)) + \
    ext2fs_le16_to_cpu((__hdr__)->eh_max) - 1)

#endif /* _LINUX_EXT3_EXTENTS */

/*
 * Journal stuff copied from e2fsprogs lib/ext2fs/kernel-jbd.h:
 */

#ifndef _LINUX_JBD_H
#define _LINUX_JBD_H

#ifndef _JFS_COMPAT_H
#define _JFS_COMPAT_H

#ifndef _LINUX_LIST_H
#define _LINUX_LIST_H

#ifndef _EXT2FS_COMPILER_H
#define _EXT2FS_COMPILER_H

#include <stddef.h>

#ifdef __GNUC__

#ifndef __GNUC_PREREQ
#if defined(__GNUC__) && defined(__GNUC_MINOR__)
#define __GNUC_PREREQ(maj, min) \
    ((__GNUC__ << 16) + __GNUC_MINOR__ >= ((maj) << 16) + (min))
#else
#define __GNUC_PREREQ(maj, min) 0
#endif
#endif

#define container_of(ptr, type, member) ({                \
    __typeof__( ((type *)0)->member ) *__mptr = (ptr);    \
    (type *)( (char *)__mptr - offsetof(type,member) );})
#else
#define container_of(ptr, type, member)                \
    ((type *)((char *)(ptr) - offsetof(type, member)))
#endif


#endif /* _EXT2FS_COMPILER_H */

/*
 * Simple doubly linked list implementation.
 *
 * Some of the internal functions ("__xxx") are useful when
 * manipulating whole lists rather than single entries, as
 * sometimes we already know the next/prev entries and we can
 * generate better code by using them directly rather than
 * using the generic single-entry routines.
 */

struct list_head {
    struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

#if (!defined(__GNUC__) && !defined(__WATCOMC__))
#define __inline__
#endif

/*
 * Insert a new entry between two known consecutive entries.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_add(struct list_head * new,
    struct list_head * prev,
    struct list_head * next)
{
    next->prev = new;
    new->next = next;
    new->prev = prev;
    prev->next = new;
}

/*
 * Insert a new entry after the specified head..
 */
static __inline__ void list_add(struct list_head *new, struct list_head *head)
{
    __list_add(new, head, head->next);
}

/*
 * Insert a new entry at the tail
 */
static __inline__ void list_add_tail(struct list_head *new, struct list_head *head)
{
    __list_add(new, head->prev, head);
}

/*
 * Delete a list entry by making the prev/next entries
 * point to each other.
 *
 * This is only for internal list manipulation where we know
 * the prev/next entries already!
 */
static __inline__ void __list_del(struct list_head * prev,
                  struct list_head * next)
{
    next->prev = prev;
    prev->next = next;
}

static __inline__ void list_del(struct list_head *entry)
{
    __list_del(entry->prev, entry->next);
}

static __inline__ int list_empty(struct list_head *head)
{
    return head->next == head;
}

/*
 * Splice in "list" into "head"
 */
static __inline__ void list_splice(struct list_head *list, struct list_head *head)
{
    struct list_head *first = list->next;

    if (first != list) {
        struct list_head *last = list->prev;
        struct list_head *at = head->next;

        first->prev = head;
        head->next = first;

        last->next = at;
        at->prev = last;
    }
}

#define list_entry(ptr, type, member) \
    container_of(ptr, type, member)

#define list_for_each(pos, head) \
        for (pos = (head)->next; pos != (head); pos = pos->next)

#endif


#include <errno.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
#ifdef HAVE_WINSOCK_H
#include <winsock.h>
#else
#include <arpa/inet.h>
#endif
#include <stdbool.h>

#define printk printf
#define KERN_ERR ""
#define KERN_DEBUG ""

#define REQ_OP_READ 0
#define REQ_OP_WRITE 1

#define cpu_to_le16(x)    ext2fs_cpu_to_le16(x)
#define cpu_to_be16(x)    ext2fs_cpu_to_be16(x)
#define cpu_to_le32(x)    ext2fs_cpu_to_le32(x)
#define cpu_to_be32(x)    ext2fs_cpu_to_be32(x)
#define cpu_to_le64(x)    ext2fs_cpu_to_le64(x)
#define cpu_to_be64(x)    ext2fs_cpu_to_be64(x)

#define le16_to_cpu(x)    ext2fs_le16_to_cpu(x)
#define be16_to_cpu(x)    ext2fs_be16_to_cpu(x)
#define le32_to_cpu(x)    ext2fs_le32_to_cpu(x)
#define be32_to_cpu(x)    ext2fs_be32_to_cpu(x)
#define le64_to_cpu(x)    ext2fs_le64_to_cpu(x)
#define be64_to_cpu(x)    ext2fs_be64_to_cpu(x)

typedef unsigned int tid_t;
typedef struct journal_s journal_t;
typedef struct kdev_s *kdev_t;

struct buffer_head;
struct inode;

typedef unsigned int gfp_t;
#define GFP_KERNEL    0
#define GFP_NOFS    0
#define __GFP_NOFAIL    0
#define JBD2_TAG_SIZE32    JBD_TAG_SIZE32
#define JBD2_BARRIER    0
typedef __u64 u64;
#define put_bh(x)    brelse(x)

#define crc32_be(x, y, z)    ext2fs_crc32_be((x), (y), (z))
#define spin_lock_init(x)
#define spin_lock(x)
#define spin_unlock(x)
#define SLAB_HWCACHE_ALIGN    0
#define SLAB_TEMPORARY        0
#define KMEM_CACHE(__struct, __flags) kmem_cache_create(#__struct,\
                sizeof(struct __struct), __alignof__(struct __struct),\
                (__flags), NULL)

#define blkdev_issue_flush(kdev)    sync_blockdev(kdev)
#define is_power_of_2(x)    ((x) != 0 && (((x) & ((x) - 1)) == 0))
#define pr_emerg(fmt)
#define pr_err(...)

enum passtype {PASS_SCAN, PASS_REVOKE, PASS_REPLAY};

#define JBD2_FC_REPLAY_STOP        0
#define JBD2_FC_REPLAY_CONTINUE        1

struct journal_s
{
    unsigned long        j_flags;
    int            j_errno;
    struct buffer_head *    j_sb_buffer;
    struct journal_superblock_s *j_superblock;
    int            j_format_version;
    unsigned long        j_head;
    unsigned long        j_tail;
    unsigned long        j_fc_first;
    unsigned long        j_fc_off;
    unsigned long        j_fc_last;
    unsigned long        j_free;
    unsigned long        j_first, j_last;
    kdev_t            j_dev;
    kdev_t            j_fs_dev;
    int            j_blocksize;
    unsigned int        j_blk_offset;
    unsigned int        j_total_len;
    struct inode *        j_inode;
    tid_t            j_tail_sequence;
    tid_t            j_transaction_sequence;
    __u8            j_uuid[16];
    struct jbd2_revoke_table_s *j_revoke;
    struct jbd2_revoke_table_s *j_revoke_table[2];
    tid_t            j_failed_commit;
    __u32            j_csum_seed;
    int (*j_fc_replay_callback)(struct journal_s *journal,
                    struct buffer_head *bh,
                    enum passtype pass, int off,
                    tid_t expected_tid);

};

#define is_journal_abort(x) 0

#define BUFFER_TRACE(bh, info)    do {} while (0)

/* Need this so we can compile with configure --enable-gcc-wall */
#ifdef NO_INLINE_FUNCS
#define inline
#endif

#endif /* _JFS_COMPAT_H */


#define JFS_DEBUG
#define jfs_debug jbd_debug

#ifndef __GNUC__
#define __FUNCTION__ ""
#endif

#define journal_oom_retry 0

#ifdef CONFIG_JBD_DEBUG
/*
 * Define JBD_EXPENSIVE_CHECKING to enable more expensive internal
 * consistency checks.  By default we don't do this unless
 * CONFIG_JBD_DEBUG is on.
 */
#define JBD_EXPENSIVE_CHECKING
extern int journal_enable_debug;
#else
#define journal_enable_debug (-1)
#endif /* !CONFIG_JBD_DEBUG */

#ifdef __STDC__
#define jbd_debug(n, f, a...)                        \
    do {                                \
        if ((n) <= journal_enable_debug) {            \
            printk (KERN_DEBUG "(%s, %d): %s: ",        \
                __FILE__, __LINE__, __FUNCTION__);    \
              printk (f, ## a);                \
        }                            \
    } while (0)
#else
#define jbd_debug(x)        /* AIX doesn't do STDC */
#endif /* !__STDC__ */

extern void * __jbd_kmalloc (char *where, size_t size, int flags, int retry);
#define jbd_kmalloc(size, flags) \
    __jbd_kmalloc(__FUNCTION__, (size), (flags), journal_oom_retry)
#define jbd_rep_kmalloc(size, flags) \
    __jbd_kmalloc(__FUNCTION__, (size), (flags), 1)

#define JBD2_MIN_JOURNAL_BLOCKS 1024
#define JBD2_DEFAULT_FAST_COMMIT_BLOCKS 256

/*
 * Internal structures used by the logging mechanism:
 */

#define JBD2_MAGIC_NUMBER 0xc03b3998U /* The first 4 bytes of /dev/random! */

/*
 * On-disk structures
 */

/*
 * Descriptor block types:
 */

#define JBD2_DESCRIPTOR_BLOCK    1
#define JBD2_COMMIT_BLOCK    2
#define JBD2_SUPERBLOCK_V1    3
#define JBD2_SUPERBLOCK_V2    4
#define JBD2_REVOKE_BLOCK    5
#define JBD2_FC_BLOCK        6

/*
 * Standard header for all descriptor blocks:
 */
typedef struct journal_header_s
{
    __be32        h_magic;
    __be32        h_blocktype;
    __be32        h_sequence;
} journal_header_t;

/*
 * Checksum types.
 */
#define JBD2_CRC32_CHKSUM   1
#define JBD2_MD5_CHKSUM     2
#define JBD2_SHA1_CHKSUM    3
#define JBD2_CRC32C_CHKSUM  4

#define JBD2_CRC32_CHKSUM_SIZE 4

#define JBD2_CHECKSUM_BYTES (32 / sizeof(__u32))
/*
 * Commit block header for storing transactional checksums:
 *
 * NOTE: If FEATURE_COMPAT_CHECKSUM (checksum v1) is set, the h_chksum*
 * fields are used to store a checksum of the descriptor and data blocks.
 *
 * If FEATURE_INCOMPAT_CSUM_V2 (checksum v2) is set, then the h_chksum
 * field is used to store crc32c(uuid+commit_block).  Each journal metadata
 * block gets its own checksum, and data block checksums are stored in
 * journal_block_tag (in the descriptor).  The other h_chksum* fields are
 * not used.
 *
 * If FEATURE_INCOMPAT_CSUM_V3 is set, the descriptor block uses
 * journal_block_tag3_t to store a full 32-bit checksum.  Everything else
 * is the same as v2.
 *
 * Checksum v1, v2, and v3 are mutually exclusive features.
 */
struct commit_header {
    __be32        h_magic;
    __be32        h_blocktype;
    __be32        h_sequence;
    unsigned char    h_chksum_type;
    unsigned char    h_chksum_size;
    unsigned char    h_padding[2];
    __be32        h_chksum[JBD2_CHECKSUM_BYTES];
    __be64        h_commit_sec;
    __be32        h_commit_nsec;
};

/*
 * The block tag: used to describe a single buffer in the journal
 */
typedef struct journal_block_tag3_s
{
    __be32        t_blocknr;    /* The on-disk block number */
    __be32        t_flags;    /* See below */
    __be32        t_blocknr_high; /* most-significant high 32bits. */
    __be32        t_checksum;    /* crc32c(uuid+seq+block) */
} journal_block_tag3_t;

typedef struct journal_block_tag_s
{
    __be32        t_blocknr;    /* The on-disk block number */
    __be16        t_checksum;    /* truncated crc32c(uuid+seq+block) */
    __be16        t_flags;    /* See below */
    __be32        t_blocknr_high; /* most-significant high 32bits. */
} journal_block_tag_t;

/* Tail of descriptor or revoke block, for checksumming */
struct jbd2_journal_block_tail {
    __be32        t_checksum;
};

/*
 * The revoke descriptor: used on disk to describe a series of blocks to
 * be revoked from the log
 */
typedef struct journal_revoke_header_s
{
    journal_header_t r_header;
    __be32         r_count;    /* Count of bytes used in the block */
} jbd2_journal_revoke_header_t;

/* Definitions for the journal tag flags word: */
#define JBD2_FLAG_ESCAPE        1    /* on-disk block is escaped */
#define JBD2_FLAG_SAME_UUID    2    /* block has same uuid as previous */
#define JBD2_FLAG_DELETED    4    /* block deleted by this transaction */
#define JBD2_FLAG_LAST_TAG    8    /* last tag in this descriptor block */


#define UUID_SIZE 16
#define JBD2_USERS_MAX 48
#define JBD2_USERS_SIZE (UUID_SIZE * JBD2_USERS_MAX)
/*
 * The journal superblock.  All fields are in big-endian byte order.
 */
typedef struct journal_superblock_s
{
/* 0x0000 */
    journal_header_t s_header;

/* 0x000C */
    /* Static information describing the journal */
    __be32    s_blocksize;        /* journal device blocksize */
    __be32    s_maxlen;        /* total blocks in journal file */
    __be32    s_first;        /* first block of log information */

/* 0x0018 */
    /* Dynamic information describing the current state of the log */
    __be32    s_sequence;        /* first commit ID expected in log */
    __be32    s_start;        /* blocknr of start of log */

/* 0x0020 */
    /* Error value, as set by journal_abort(). */
    __s32    s_errno;

/* 0x0024 */
    /* Remaining fields are only valid in a version-2 superblock */
    __be32    s_feature_compat;     /* compatible feature set */
    __be32    s_feature_incompat;     /* incompatible feature set */
    __be32    s_feature_ro_compat;     /* readonly-compatible feature set */
/* 0x0030 */
    __u8    s_uuid[16];        /* 128-bit uuid for journal */

/* 0x0040 */
    __be32    s_nr_users;        /* Nr of filesystems sharing log */

    __be32    s_dynsuper;        /* Blocknr of dynamic superblock copy*/

/* 0x0048 */
    __be32    s_max_transaction;    /* Limit of journal blocks per trans.*/
    __be32    s_max_trans_data;    /* Limit of data blocks per trans. */

/* 0x0050 */
    __u8    s_checksum_type;    /* checksum type */
    __u8    s_padding2[3];
/* 0x0054 */
    __be32    s_num_fc_blks;        /* Number of fast commit blocks */
/* 0x0058 */
    __be32    s_padding[41];
    __be32    s_checksum;        /* crc32c(superblock) */

/* 0x0100 */
    __u8    s_users[JBD2_USERS_SIZE];        /* ids of all fs'es sharing the log */

/* 0x0400 */
} journal_superblock_t;

#define JBD2_HAS_COMPAT_FEATURE(j,mask)                    \
    ((j)->j_format_version >= 2 &&                    \
     ((j)->j_superblock->s_feature_compat & ext2fs_cpu_to_be32((mask))))
#define JBD2_HAS_RO_COMPAT_FEATURE(j,mask)                \
    ((j)->j_format_version >= 2 &&                    \
     ((j)->j_superblock->s_feature_ro_compat & ext2fs_cpu_to_be32((mask))))
#define JBD2_HAS_INCOMPAT_FEATURE(j,mask)                \
    ((j)->j_format_version >= 2 &&                    \
     ((j)->j_superblock->s_feature_incompat & ext2fs_cpu_to_be32((mask))))

#define JBD2_FEATURE_COMPAT_CHECKSUM        0x00000001

#define JBD2_FEATURE_INCOMPAT_REVOKE        0x00000001
#define JBD2_FEATURE_INCOMPAT_64BIT        0x00000002
#define JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT    0x00000004
#define JBD2_FEATURE_INCOMPAT_CSUM_V2        0x00000008
#define JBD2_FEATURE_INCOMPAT_CSUM_V3        0x00000010
#define JBD2_FEATURE_INCOMPAT_FAST_COMMIT    0x00000020

/* Features known to this kernel version: */
#define JBD2_KNOWN_COMPAT_FEATURES    0
#define JBD2_KNOWN_ROCOMPAT_FEATURES    0
#define JBD2_KNOWN_INCOMPAT_FEATURES    (JBD2_FEATURE_INCOMPAT_REVOKE|\
                     JBD2_FEATURE_INCOMPAT_ASYNC_COMMIT| \
                     JBD2_FEATURE_INCOMPAT_64BIT|\
                     JBD2_FEATURE_INCOMPAT_CSUM_V2|    \
                     JBD2_FEATURE_INCOMPAT_CSUM_V3 | \
                     JBD2_FEATURE_INCOMPAT_FAST_COMMIT)

#ifdef NO_INLINE_FUNCS
extern size_t journal_tag_bytes(journal_t *journal);
extern int jbd2_journal_has_csum_v2or3(journal_t *journal);
extern int jbd2_journal_get_num_fc_blks(journal_superblock_t *jsb);
extern int tid_gt(tid_t x, tid_t y) EXT2FS_ATTR((unused));
extern int tid_geq(tid_t x, tid_t y) EXT2FS_ATTR((unused));
#endif

#if (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef E2FSCK_INCLUDE_INLINE_FUNCS
#if (__STDC_VERSION__ >= 199901L)
#define _INLINE_ extern inline
#else
#define _INLINE_ inline
#endif
#else /* !E2FSCK_INCLUDE_INLINE FUNCS */
#if (__STDC_VERSION__ >= 199901L)
#define _INLINE_ inline
#else /* not C99 */
#ifdef __GNUC__
#define _INLINE_ extern __inline__
#else                /* For Watcom C */
#define _INLINE_ extern inline
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* INCLUDE_INLINE_FUNCS */

#define ext2fs_cpu_to_be32(x) ((__force __be32)(__u32)(x))
#define ext2fs_be32_to_cpu(x) ((__force __u32)(__be32)(x))

/* journal feature predicate functions */
#define JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
_INLINE_ int jbd2_has_feature_##name(journal_t *j); \
_INLINE_ int jbd2_has_feature_##name(journal_t *j) \
{ \
    return ((j)->j_format_version >= 2 && \
        ((j)->j_superblock->s_feature_compat & \
         ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname)) != 0); \
} \
_INLINE_ void jbd2_set_feature_##name(journal_t *j); \
_INLINE_ void jbd2_set_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_compat |= \
        ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
} \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_compat &= \
        ~ext2fs_cpu_to_be32(JBD2_FEATURE_COMPAT_##flagname); \
}

#define JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
_INLINE_ int jbd2_has_feature_##name(journal_t *j);    \
_INLINE_ int jbd2_has_feature_##name(journal_t *j) \
{ \
    return ((j)->j_format_version >= 2 && \
        ((j)->j_superblock->s_feature_ro_compat & \
         ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname)) != 0); \
} \
_INLINE_ void jbd2_set_feature_##name(journal_t *j); \
_INLINE_ void jbd2_set_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_ro_compat |= \
        ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
} \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_ro_compat &= \
        ~ext2fs_cpu_to_be32(JBD2_FEATURE_RO_COMPAT_##flagname); \
}

#define JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
_INLINE_ int jbd2_has_feature_##name(journal_t *j); \
_INLINE_ int jbd2_has_feature_##name(journal_t *j) \
{ \
    return ((j)->j_format_version >= 2 && \
        ((j)->j_superblock->s_feature_incompat & \
         ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname)) != 0); \
} \
_INLINE_ void jbd2_set_feature_##name(journal_t *j); \
_INLINE_ void jbd2_set_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_incompat |= \
        ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
} \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j); \
_INLINE_ void jbd2_clear_feature_##name(journal_t *j) \
{ \
    (j)->j_superblock->s_feature_incompat &= \
        ~ext2fs_cpu_to_be32(JBD2_FEATURE_INCOMPAT_##flagname); \
}

#else
#define JBD2_FEATURE_COMPAT_FUNCS(name, flagname) \
extern int jbd2_has_feature_##name(journal_t *j); \
extern void jbd2_set_feature_##name(journal_t *j); \
extern void jbd2_clear_feature_##name(journal_t *j);

#define JBD2_FEATURE_RO_COMPAT_FUNCS(name, flagname) \
extern int jbd2_has_feature_##name(journal_t *j); \
extern void jbd2_set_feature_##name(journal_t *j); \
extern void jbd2_clear_feature_##name(journal_t *j);

#define JBD2_FEATURE_INCOMPAT_FUNCS(name, flagname) \
extern int jbd2_has_feature_##name(journal_t *j); \
extern void jbd2_set_feature_##name(journal_t *j); \
extern void jbd2_clear_feature_##name(journal_t *j);

#endif /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

JBD2_FEATURE_COMPAT_FUNCS(checksum,        CHECKSUM)

JBD2_FEATURE_INCOMPAT_FUNCS(revoke,        REVOKE)
JBD2_FEATURE_INCOMPAT_FUNCS(64bit,        64BIT)
JBD2_FEATURE_INCOMPAT_FUNCS(async_commit,    ASYNC_COMMIT)
JBD2_FEATURE_INCOMPAT_FUNCS(csum2,        CSUM_V2)
JBD2_FEATURE_INCOMPAT_FUNCS(csum3,        CSUM_V3)
JBD2_FEATURE_INCOMPAT_FUNCS(fast_commit,    FAST_COMMIT)

#if (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
/*
 * helper functions to deal with 32 or 64bit block numbers.
 */
_INLINE_ size_t journal_tag_bytes(journal_t *journal)
{
    size_t sz;

    if (jbd2_has_feature_csum3(journal))
        return sizeof(journal_block_tag3_t);

    sz = sizeof(journal_block_tag_t);

    if (jbd2_has_feature_csum2(journal))
        sz += sizeof(__u16);

    if (jbd2_has_feature_64bit(journal))
        return sz;

    return sz - sizeof(__u32);
}

_INLINE_ int jbd2_journal_has_csum_v2or3(journal_t *journal)
{
    if (jbd2_has_feature_csum2(journal) || jbd2_has_feature_csum3(journal))
        return 1;

    return 0;
}

_INLINE_ int jbd2_journal_get_num_fc_blks(journal_superblock_t *jsb)
{
    int num_fc_blocks = be32_to_cpu(jsb->s_num_fc_blks);

    return num_fc_blocks ? num_fc_blocks : JBD2_DEFAULT_FAST_COMMIT_BLOCKS;
}

/* Comparison functions for transaction IDs: perform comparisons using
 * modulo arithmetic so that they work over sequence number wraps. */

_INLINE_ int tid_gt(tid_t x, tid_t y)
{
    int difference = (x - y);
    return (difference > 0);
}

_INLINE_ int tid_geq(tid_t x, tid_t y)
{
    int difference = (x - y);
    return (difference >= 0);
}
#endif /* (defined(E2FSCK_INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS)) */

#undef _INLINE_

extern int journal_blocks_per_page(struct inode *inode);

/*
 * Definitions which augment the buffer_head layer
 */

/* journaling buffer types */
#define BJ_None        0    /* Not journaled */
#define BJ_SyncData    1    /* Normal data: flush before commit */
#define BJ_AsyncData    2    /* writepage data: wait on it before commit */
#define BJ_Metadata    3    /* Normal journaled metadata */
#define BJ_Forget    4    /* Buffer superceded by this transaction */
#define BJ_IO        5    /* Buffer is for temporary IO use */
#define BJ_Shadow    6    /* Buffer contents being shadowed to the log */
#define BJ_LogCtl    7    /* Buffer contains log descriptors */
#define BJ_Reserved    8    /* Buffer is reserved for access by journal */
#define BJ_Types    9

extern int jbd_blocks_per_page(struct inode *inode);

#endif    /* _LINUX_JBD_H */

#endif /* _EXT4_ANALYZE_H_ */
