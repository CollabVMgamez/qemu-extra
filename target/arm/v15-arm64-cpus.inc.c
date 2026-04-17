static void aarch64_apple_m1_ultra_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,m1-ultra";
    cpu->midr = 1632567328;
    cpu->ctr = 0x8444c004;
}


static void aarch64_apple_m4_pro_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "apple,m4-pro";
    cpu->midr = 1640955936;
    cpu->ctr = 0x8444c004;
}


static void aarch64_snapdragon_x2_elite_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,x2-elite";
    cpu->midr = 1896808720;
    cpu->ctr = 0x8444c004;
}


static void aarch64_snapdragon_x_plus_8_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,x-plus-8";
    cpu->midr = 1896808736;
    cpu->ctr = 0x8444c004;
}


static void aarch64_snapdragon_8cx_gen4_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,8cx-gen4";
    cpu->midr = 1896808480;
    cpu->ctr = 0x8444c004;
}


static void aarch64_snapdragon_7c_gen2_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "qualcomm,7c-gen2";
    cpu->midr = 1896808496;
    cpu->ctr = 0x8444c004;
}


static void aarch64_graviton1_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton";
    cpu->midr = 3499032576;
    cpu->ctr = 0x8444c004;
}


static void aarch64_graviton2_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton2";
    cpu->midr = 3502178304;
    cpu->ctr = 0x8444c004;
}


static void aarch64_graviton3c_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "amazon,graviton3c";
    cpu->midr = 3503226880;
    cpu->ctr = 0x8444c004;
}


static void aarch64_ampereone_320_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone-320";
    cpu->midr = 3280928768;
    cpu->ctr = 0x8444c004;
}


static void aarch64_phytium_ft2000_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "phytium,ft-2000+";
    cpu->midr = 1712259072;
    cpu->ctr = 0x8444c004;
}


static void aarch64_phytium_s2500_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "phytium,s2500";
    cpu->midr = 1713307648;
    cpu->ctr = 0x8444c004;
}


static void aarch64_phytium_d2000_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "phytium,d2000";
    cpu->midr = 1714356224;
    cpu->ctr = 0x8444c004;
}


static void aarch64_phytium_ft2000ah_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "phytium,ft-2000ah";
    cpu->midr = 1715404800;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rockchip_rk3588_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3588";
    cpu->midr = 1093648514;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rockchip_rk3588s_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3588s";
    cpu->midr = 1093648515;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rockchip_rk3568_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3568";
    cpu->midr = 1093648516;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rockchip_rk3566_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3566";
    cpu->midr = 1093648517;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rockchip_rk3562_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "rockchip,rk3562";
    cpu->midr = 1093648518;
    cpu->ctr = 0x8444c004;
}


static void aarch64_dimensity_8000_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "mediatek,mt6893";
    cpu->midr = 1091556480;
    cpu->ctr = 0x8444c004;
}


static void aarch64_dimensity_8100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "mediatek,mt6895";
    cpu->midr = 1091556481;
    cpu->ctr = 0x8444c004;
}


static void aarch64_kunpeng930s_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kunpeng930s";
    cpu->midr = 1763639296;
    cpu->ctr = 0x8444c004;
    cpu->sve_max_vq = 4;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}


static void aarch64_kunpeng920s_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "hisilicon,kunpeng920s";
    cpu->midr = 1762590721;
    cpu->ctr = 0x8444c004;
    cpu->sve_max_vq = 4;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}


static void aarch64_layerscape2085_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls2085a";
    cpu->midr = 1091555457;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape1086_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1086a";
    cpu->midr = 1091555458;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape1028_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1028a";
    cpu->midr = 1091555459;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape1046_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1046a";
    cpu->midr = 1091555460;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape1043_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1043a";
    cpu->midr = 1091555461;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape1012_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,ls1012a";
    cpu->midr = 1091555462;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape2040_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,lx2160a";
    cpu->midr = 1091555463;
    cpu->ctr = 0x8444c004;
}


static void aarch64_layerscape2160_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,lx2160a";
    cpu->midr = 1091555464;
    cpu->ctr = 0x8444c004;
}


static void aarch64_thunderx3_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cavium,thunderx3";
    cpu->midr = 2702114816;
    cpu->ctr = 0x8444c004;
}


static void aarch64_thunderx88_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "cavium,thunderx-88xx";
    cpu->midr = 2702114817;
    cpu->ctr = 0x8444c004;
}


static void aarch64_altra_qemu_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,altra-qemu";
    cpu->midr = 3230597121;
    cpu->ctr = 0x8444c004;
}


static void aarch64_neoverse_n2_sve_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-n2-sve";
    cpu->midr = 3569340464;
    cpu->ctr = 0x8444c004;
    cpu->sve_max_vq = 4;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}


static void aarch64_neoverse_v3_sve_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v3-sve";
    cpu->midr = 3636449328;
    cpu->ctr = 0x8444c004;
    cpu->sve_max_vq = 4;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}


static void aarch64_neoverse_v3ae_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,neoverse-v3ae";
    cpu->midr = 3636449329;
    cpu->ctr = 0x8444c004;
    cpu->sve_max_vq = 4;
    object_property_set_uint(obj, "sve-max-vq", 4, &error_abort);
}


static void aarch64_corstone1000_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,corstone-1000";
    cpu->midr = 1091556352;
    cpu->ctr = 0x8444c004;
}


static void aarch64_totalcompute_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "arm,totalcompute";
    cpu->midr = 3502178305;
    cpu->ctr = 0x8444c004;
}


static void aarch64_tensor_g1_m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "google,tensor-g1";
    cpu->midr = 1091555520;
    cpu->ctr = 0x8444c004;
}


static void aarch64_tensor_g2_m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "google,tensor-g2";
    cpu->midr = 1091555521;
    cpu->ctr = 0x8444c004;
}


static void aarch64_tensor_g3_m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "google,tensor-g3";
    cpu->midr = 1091555522;
    cpu->ctr = 0x8444c004;
}


static void aarch64_tensor_g4_m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "google,tensor-g4";
    cpu->midr = 1091555523;
    cpu->ctr = 0x8444c004;
}


static void aarch64_tensor_g5_m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "google,tensor-g5";
    cpu->midr = 1091555524;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx8m_quad_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx8m-quad";
    cpu->midr = 1091555394;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx8m_plus_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx8mp";
    cpu->midr = 1091555395;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx8m_mini_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx8mm";
    cpu->midr = 1091555396;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx8m_nano_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx8mn";
    cpu->midr = 1091555397;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx93_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx93";
    cpu->midr = 1091555398;
    cpu->ctr = 0x8444c004;
}


static void aarch64_imx91_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "fsl,imx91";
    cpu->midr = 1091555399;
    cpu->ctr = 0x8444c004;
}


static void aarch64_j722s_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,j722s";
    cpu->midr = 1091555408;
    cpu->ctr = 0x8444c004;
}


static void aarch64_am625_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,am625";
    cpu->midr = 1091555409;
    cpu->ctr = 0x8444c004;
}


static void aarch64_am67a_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ti,am67a";
    cpu->midr = 1091555410;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rcar_h3_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "renesas,r8a7795";
    cpu->midr = 1091555424;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rcar_m3_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "renesas,r8a7796";
    cpu->midr = 1091555425;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rcar_v3m_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "renesas,r8a77970";
    cpu->midr = 1091555426;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rcar_v3h_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "renesas,r8a77980";
    cpu->midr = 1091555427;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rcar_s4_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "renesas,r8a779f0";
    cpu->midr = 1091555428;
    cpu->ctr = 0x8444c004;
}


static void aarch64_bcm2711_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "brcm,bcm2711";
    cpu->midr = 1091555459;
    cpu->ctr = 0x8444c004;
}


static void aarch64_bcm2712_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "brcm,bcm2712";
    cpu->midr = 1091555460;
    cpu->ctr = 0x8444c004;
}


static void aarch64_bcm2837_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "brcm,bcm2837";
    cpu->midr = 1091555520;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rtd1619_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "realtek,rtd1619";
    cpu->midr = 1091555440;
    cpu->ctr = 0x8444c004;
}


static void aarch64_rtd1319_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "realtek,rtd1319";
    cpu->midr = 1091555441;
    cpu->ctr = 0x8444c004;
}


static void aarch64_allwinner_a523_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "allwinner,a523";
    cpu->midr = 1091555456;
    cpu->ctr = 0x8444c004;
}


static void aarch64_allwinner_t507_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "allwinner,t507";
    cpu->midr = 1091555457;
    cpu->ctr = 0x8444c004;
}


static void aarch64_cobalt_100_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "microsoft,cobalt-100";
    cpu->midr = 3502178320;
    cpu->ctr = 0x8444c004;
}


static void aarch64_ampereone_3g_initfn(Object *obj)
{
    ARMCPU *cpu = ARM_CPU(obj);
    cpu->dtb_compatible = "ampere,ampereone-3g";
    cpu->midr = 3283025920;
    cpu->ctr = 0x8444c004;
}


