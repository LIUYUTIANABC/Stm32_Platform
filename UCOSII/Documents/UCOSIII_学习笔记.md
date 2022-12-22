## 疑问与解答

- 任务是如何调度的？


OS_PRIO_TBL_SIZE          ((OS_CFG_PRIO_MAX - 1u) / (DEF_INT_CPU_NBR_BITS) + 1u)

OS_CFG_PRIO_MAX 32u

DEF_INT_CPU_NBR_BITS                           (CPU_CFG_DATA_SIZE     * DEF_OCTET_NBR_BITS)

CPU_CFG_DATA_SIZE              CPU_WORD_SIZE_32
CPU_WORD_SIZE_32                          4
DEF_OCTET_NBR_BITS                                8u


