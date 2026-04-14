/*
 * QEMU SMBus EEPROM device
 *
 * Copyright (c) 2007 Arastra, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qapi/error.h"
#include "hw/core/boards.h"
#include "hw/i2c/i2c.h"
#include "hw/i2c/smbus_slave.h"
#include "hw/core/qdev-properties.h"
#include "migration/vmstate.h"
#include "hw/i2c/smbus_eeprom.h"
#include "qom/object.h"

//#define DEBUG

#define TYPE_SMBUS_EEPROM "smbus-eeprom"

OBJECT_DECLARE_SIMPLE_TYPE(SMBusEEPROMDevice, SMBUS_EEPROM)

#define SMBUS_EEPROM_SIZE 256

struct SMBusEEPROMDevice {
    SMBusDevice smbusdev;
    uint8_t data[SMBUS_EEPROM_SIZE];
    uint8_t *init_data;
    uint8_t offset;
    bool accessed;
};

static uint8_t eeprom_receive_byte(SMBusDevice *dev)
{
    SMBusEEPROMDevice *eeprom = SMBUS_EEPROM(dev);
    uint8_t *data = eeprom->data;
    uint8_t val = data[eeprom->offset++];

    eeprom->accessed = true;
#ifdef DEBUG
    printf("eeprom_receive_byte: addr=0x%02x val=0x%02x\n",
           dev->i2c.address, val);
#endif
    return val;
}

static int eeprom_write_data(SMBusDevice *dev, uint8_t *buf, uint8_t len)
{
    SMBusEEPROMDevice *eeprom = SMBUS_EEPROM(dev);
    uint8_t *data = eeprom->data;

    eeprom->accessed = true;
#ifdef DEBUG
    printf("eeprom_write_byte: addr=0x%02x cmd=0x%02x val=0x%02x\n",
           dev->i2c.address, buf[0], buf[1]);
#endif
    /* len is guaranteed to be > 0 */
    eeprom->offset = buf[0];
    buf++;
    len--;

    for (; len > 0; len--) {
        data[eeprom->offset] = *buf++;
        eeprom->offset = (eeprom->offset + 1) % SMBUS_EEPROM_SIZE;
    }

    return 0;
}

static bool smbus_eeprom_vmstate_needed(void *opaque)
{
    SMBusEEPROMDevice *eeprom = opaque;

    return eeprom->accessed || smbus_vmstate_needed(&eeprom->smbusdev);
}

static const VMStateDescription vmstate_smbus_eeprom = {
    .name = "smbus-eeprom",
    .version_id = 1,
    .minimum_version_id = 1,
    .needed = smbus_eeprom_vmstate_needed,
    .fields = (const VMStateField[]) {
        VMSTATE_SMBUS_DEVICE(smbusdev, SMBusEEPROMDevice),
        VMSTATE_UINT8_ARRAY(data, SMBusEEPROMDevice, SMBUS_EEPROM_SIZE),
        VMSTATE_UINT8(offset, SMBusEEPROMDevice),
        VMSTATE_BOOL(accessed, SMBusEEPROMDevice),
        VMSTATE_END_OF_LIST()
    }
};

/*
 * Reset the EEPROM contents to the initial state on a reset.  This
 * isn't really how an EEPROM works, of course, but the general
 * principle of QEMU is to restore function on reset to what it would
 * be if QEMU was stopped and started.
 *
 * The proper thing to do would be to have a backing blockdev to hold
 * the contents and restore that on startup, and not do this on reset.
 * But until that time, act as if we had been stopped and restarted.
 */
static void smbus_eeprom_reset(DeviceState *dev)
{
    SMBusEEPROMDevice *eeprom = SMBUS_EEPROM(dev);

    memcpy(eeprom->data, eeprom->init_data, SMBUS_EEPROM_SIZE);
    eeprom->offset = 0;
}

static void smbus_eeprom_realize(DeviceState *dev, Error **errp)
{
    SMBusEEPROMDevice *eeprom = SMBUS_EEPROM(dev);

    smbus_eeprom_reset(dev);
    if (eeprom->init_data == NULL) {
        error_setg(errp, "init_data cannot be NULL");
    }
}

static void smbus_eeprom_class_initfn(ObjectClass *klass, const void *data)
{
    DeviceClass *dc = DEVICE_CLASS(klass);
    SMBusDeviceClass *sc = SMBUS_DEVICE_CLASS(klass);

    dc->realize = smbus_eeprom_realize;
    device_class_set_legacy_reset(dc, smbus_eeprom_reset);
    sc->receive_byte = eeprom_receive_byte;
    sc->write_data = eeprom_write_data;
    dc->vmsd = &vmstate_smbus_eeprom;
    /* Reason: init_data */
    dc->user_creatable = false;
}

static const TypeInfo smbus_eeprom_types[] = {
    {
        .name          = TYPE_SMBUS_EEPROM,
        .parent        = TYPE_SMBUS_DEVICE,
        .instance_size = sizeof(SMBusEEPROMDevice),
        .class_init    = smbus_eeprom_class_initfn,
    },
};

DEFINE_TYPES(smbus_eeprom_types)

void smbus_eeprom_init_one(I2CBus *smbus, uint8_t address, uint8_t *eeprom_buf)
{
    DeviceState *dev;

    dev = qdev_new(TYPE_SMBUS_EEPROM);
    qdev_prop_set_uint8(dev, "address", address);
    /* FIXME: use an array of byte or block backend property? */
    SMBUS_EEPROM(dev)->init_data = eeprom_buf;
    qdev_realize_and_unref(dev, (BusState *)smbus, &error_fatal);
}

void smbus_eeprom_init(I2CBus *smbus, int nb_eeprom,
                       const uint8_t *eeprom_spd, int eeprom_spd_size)
{
    int i;
     /* XXX: make this persistent */

    assert(nb_eeprom <= 8);
    uint8_t *eeprom_buf = g_malloc0(8 * SMBUS_EEPROM_SIZE);
    if (eeprom_spd_size > 0) {
        memcpy(eeprom_buf, eeprom_spd, eeprom_spd_size);
    }

    for (i = 0; i < nb_eeprom; i++) {
        smbus_eeprom_init_one(smbus, 0x50 + i,
                              eeprom_buf + (i * SMBUS_EEPROM_SIZE));
    }
}

/* Generate SDRAM SPD EEPROM data describing a module of type and size */
uint8_t *spd_data_generate(enum sdram_type type, ram_addr_t ram_size)
{
    uint8_t *spd;
    uint8_t nbanks;
    uint16_t density;
    uint32_t size;
    int min_log2, max_log2, sz_log2;
    int i;

    /*
     * DDR3 and DDR4 use a completely different SPD byte layout (JEDEC
     * SPD for DDR3 = 256 bytes; DDR4 = 512 bytes).  We generate a
     * synthetic SPD that is minimal but sufficient for CPU-Z to detect
     * the correct type, speed, and size.
     */
    if (type == DDR3) {
        uint8_t *spd3 = g_malloc0(256);
        uint32_t sz_mb = ram_size >> 20;
        /* SPD byte layout for DDR3 (JEDEC SPD spec rev 1.1) */
        spd3[0]  = 0x92;  /* bytes used = 146, total = 256 */
        spd3[1]  = 0x10;  /* SPD revision 1.0 */
        spd3[2]  = DDR3;  /* key byte / DRAM type */
        spd3[3]  = 0x02;  /* key byte / module type: UDIMM */
        /* Bank address bits [3:2], SDRAM capacity [3:0]: 0x04=4Gb die */
        spd3[4]  = (sz_mb >= 2048) ? 0x04 : 0x03;
        spd3[5]  = 0x11;  /* row=15 bits [5:3], col=10 bits [2:0] */
        spd3[6]  = 0x02;  /* nominal voltage: 1.5V */
        spd3[7]  = 0x08;  /* bus width: 64-bit */
        spd3[8]  = 0x08;  /* bus width extension: 0 (non-ECC) */
        spd3[9]  = 0x11;  /* fine timebase / medium timebase */
        spd3[10] = 0x08;  /* tCKavg min (8 = 1.25ns = DDR3-1600) */
        spd3[11] = 0x00;
        spd3[12] = 0x82;  /* CAS latencies supported [17:10] */
        spd3[13] = 0x08;
        spd3[14] = 0x00;  /* min CAS latency time */
        spd3[15] = 0x00;  /* min write recovery time */
        spd3[17] = 0x69;  /* tAA min = 13.125ns (CL9 @ DDR3-1600) */
        spd3[18] = 0x78;  /* tWR min */
        spd3[19] = 0x69;  /* tRCD min */
        spd3[20] = 0x3C;  /* tRRD min */
        spd3[21] = 0x69;  /* tRP min */
        spd3[22] = 0x11;  /* tRAS/tRC upper nibbles */
        spd3[23] = 0x20;  /* tRAS min LSB */
        spd3[24] = 0x89;  /* tRC min LSB */
        spd3[25] = 0x00;  /* tRFC1 min LSB */
        spd3[26] = 0x05;  /* tRFC1 min MSB */
        spd3[27] = 0x3C;  /* tFAW upper/lower */
        spd3[28] = 0x3C;
        /* Module size: ranks [5:3], DRAM width [2:0] */
        spd3[7]  = 0x08;  /* 64-bit bus width */
        spd3[13] = 0x08;  /* fine CAS latencies */
        /* DIMM size encoding */
        spd3[4]  = (sz_mb >= 4096) ? 0x05 :
                   (sz_mb >= 2048) ? 0x04 :
                   (sz_mb >= 1024) ? 0x03 : 0x02;
        /* checksum bytes 0-116 */
        uint8_t crc = 0;
        for (i = 0; i < 126; i++) crc += spd3[i];
        spd3[126] = crc;
        return spd3;
    }

    if (type == DDR5 || type == LPDDR4 || type == LPDDR5) {
        /* DDR5/LPDDR4/LPDDR5 SPD (512 bytes, JEDEC SPD5 layout)  */
        uint8_t *spd5 = g_malloc0(512);
        uint32_t sz_mb = ram_size >> 20;
        spd5[0]  = 0x80;
        spd5[1]  = 0x10;
        spd5[2]  = (uint8_t)type;
        spd5[3]  = (type == LPDDR4 || type == LPDDR5) ? 0x03 : 0x02;
        spd5[4]  = (sz_mb >= 8192) ? 0x06 :
                   (sz_mb >= 4096) ? 0x05 : 0x04;
        spd5[5]  = 0x11;
        spd5[6]  = 0x08;
        spd5[7]  = 0x08;
        spd5[14] = 0x03;
        spd5[20] = 0x14;  /* tCKavg min DDR5-4800 */
        uint8_t csum5 = 0;
        for (int ci = 0; ci < 126; ci++) csum5 += spd5[ci];
        spd5[126] = csum5;
        return spd5;
    }

    if (type == DDR4) {
        uint8_t *spd4 = g_malloc0(512);
        uint32_t sz_mb = ram_size >> 20;
        /* SPD byte layout for DDR4 (JEDEC SPD spec rev 1.0) */
        spd4[0]  = 0x23;  /* bytes used = 384 */
        spd4[1]  = 0x10;  /* SPD revision 1.0 */
        spd4[2]  = DDR4;  /* key byte / DRAM type */
        spd4[3]  = 0x01;  /* key byte / module type: RDIMM (0x01) */
        spd4[4]  = (sz_mb >= 4096) ? 0x05 :
                   (sz_mb >= 2048) ? 0x04 : 0x03; /* bank/capacity */
        spd4[5]  = 0x11;  /* row bits [5:3], col bits [2:0] */
        spd4[6]  = 0x01;  /* primary bus width: 8 bits */
        spd4[7]  = 0x08;  /* bus width: 64-bit */
        spd4[8]  = 0x00;  /* bus width extension: ECC=0 */
        spd4[9]  = 0x11;  /* fine/medium timebase */
        spd4[10] = 0x08;  /* tCKavg min = 0.625ns -> DDR4-2133 */
        spd4[12] = 0xFC;  /* CAS latencies: 10-17 supported */
        spd4[13] = 0x01;
        spd4[14] = 0x00;
        spd4[15] = 0x00;
        spd4[16] = 0x00;  /* min tAA */
        spd4[17] = 0x6E;  /* tRCD min */
        spd4[18] = 0x6E;  /* tRP min */
        spd4[19] = 0x11;  /* tRAS/tRC upper nibbles */
        spd4[20] = 0x18;  /* tRAS min LSB */
        spd4[21] = 0x86;  /* tRC min LSB */
        /* Module organization: [5:3]=ranks, [2:0]=DRAM width */
        spd4[12] = 0xFC;
        /* Simple CRC-16 for bytes 0-125 (SPD4 uses CRC16, not sum) */
        uint16_t crc16 = 0;
        for (i = 0; i < 126; i++) {
            crc16 ^= (uint16_t)spd4[i] << 8;
            for (int j = 0; j < 8; j++)
                crc16 = (crc16 & 0x8000) ? (crc16 << 1) ^ 0x1021 : crc16 << 1;
        }
        spd4[126] = crc16 & 0xFF;
        spd4[127] = (crc16 >> 8) & 0xFF;
        return spd4;
    }

    switch (type) {
    case SDR:
        min_log2 = 2;
        max_log2 = 9;
        break;
    case DDR:
        min_log2 = 5;
        max_log2 = 12;
        break;
    case DDR2:
        min_log2 = 7;
        max_log2 = 14;
        break;
    default:
        /* DDR3/DDR4/DDR5/LPDDR handled above; treat anything else as DDR2 */
        min_log2 = 7;
        max_log2 = 14;
        break;
    }
    size = ram_size >> 20; /* work in terms of megabytes */
    sz_log2 = 31 - clz32(size);
    size = 1U << sz_log2;
    assert(ram_size == size * MiB);
    assert(sz_log2 >= min_log2);

    nbanks = 1;
    while (sz_log2 > max_log2 && nbanks < 8) {
        sz_log2--;
        nbanks *= 2;
    }

    assert(size == (1ULL << sz_log2) * nbanks);

    /* split to 2 banks if possible to avoid a bug in MIPS Malta firmware */
    if (nbanks == 1 && sz_log2 > min_log2) {
        sz_log2--;
        nbanks++;
    }

    density = 1ULL << (sz_log2 - 2);
    switch (type) {
    case DDR2:
        density = (density & 0xe0) | (density >> 8 & 0x1f);
        break;
    case DDR:
        density = (density & 0xf8) | (density >> 8 & 0x07);
        break;
    case SDR:
    default:
        density &= 0xff;
        break;
    }

    spd = g_malloc0(256);
    spd[0] = 128;   /* data bytes in EEPROM */
    spd[1] = 8;     /* log2 size of EEPROM */
    spd[2] = type;
    spd[3] = 13;    /* row address bits */
    spd[4] = 10;    /* column address bits */
    spd[5] = (type == DDR2 ? nbanks - 1 : nbanks);
    spd[6] = 64;    /* module data width */
                    /* reserved / data width high */
    spd[8] = 4;     /* interface voltage level */
    spd[9] = 0x25;  /* highest CAS latency */
    spd[10] = 1;    /* access time */
                    /* DIMM configuration 0 = non-ECC */
    spd[12] = 0x82; /* refresh requirements */
    spd[13] = 8;    /* primary SDRAM width */
                    /* ECC SDRAM width */
    spd[15] = (type == DDR2 ? 0 : 1); /* reserved / delay for random col rd */
    spd[16] = 12;   /* burst lengths supported */
    spd[17] = 4;    /* banks per SDRAM device */
    spd[18] = 12;   /* ~CAS latencies supported */
    spd[19] = (type == DDR2 ? 0 : 1); /* reserved / ~CS latencies supported */
    spd[20] = 2;    /* DIMM type / ~WE latencies */
    spd[21] = (type < DDR2 ? 0x20 : 0); /* module features */
                    /* memory chip features */
    spd[23] = 0x12; /* clock cycle time @ medium CAS latency */
                    /* data access time */
                    /* clock cycle time @ short CAS latency */
                    /* data access time */
    spd[27] = 20;   /* min. row precharge time */
    spd[28] = 15;   /* min. row active row delay */
    spd[29] = 20;   /* min. ~RAS to ~CAS delay */
    spd[30] = 45;   /* min. active to precharge time */
    spd[31] = density;
    spd[32] = 20;   /* addr/cmd setup time */
    spd[33] = 8;    /* addr/cmd hold time */
    spd[34] = 20;   /* data input setup time */
    spd[35] = 8;    /* data input hold time */
    spd[36] = (type == DDR2 ? 13 << 2 : 0); /* min. write recovery time */

    /* checksum */
    for (i = 0; i < 63; i++) {
        spd[63] += spd[i];
    }
    return spd;
}
