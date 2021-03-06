/**********************************************************************************************************************
 *  COPYRIGHT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  Copyright (c) 2018 by Vector Informatik GmbH. All rights reserved.
 *
 *                This software is copyright protected and proprietary to Vector Informatik GmbH.
 *                Vector Informatik GmbH grants to you only those rights as set out in the license conditions.
 *                All other rights remain with Vector Informatik GmbH.
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -----------------------------------------------------------------------------------------------------------------*/
/**        \file  crc.cc
 *        \brief  TODO
 *
 *      \details  TODO
 *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include "ara/crc/crc.h"
#include <array>
#include <cassert>

namespace ara {
namespace crc {
namespace internal {

/**
 * \brief Look-up table for CRC with polynomial 0xF4ACFB13
 * reflected = true
 * \remark Generated from http://www.sunshine2k.de/coding/javascript/crc/crc_js.html
 */
constexpr std::array<std::uint32_t, 256U> kLookUpTableCrc32P04{
    0x00000000U, 0x30850FF5U, 0x610A1FEAU, 0x518F101FU, 0xC2143FD4U, 0xF2913021U, 0xA31E203EU, 0x939B2FCBU, 0x159615F7U,
    0x25131A02U, 0x749C0A1DU, 0x441905E8U, 0xD7822A23U, 0xE70725D6U, 0xB68835C9U, 0x860D3A3CU, 0x2B2C2BEEU, 0x1BA9241BU,
    0x4A263404U, 0x7AA33BF1U, 0xE938143AU, 0xD9BD1BCFU, 0x88320BD0U, 0xB8B70425U, 0x3EBA3E19U, 0x0E3F31ECU, 0x5FB021F3U,
    0x6F352E06U, 0xFCAE01CDU, 0xCC2B0E38U, 0x9DA41E27U, 0xAD2111D2U, 0x565857DCU, 0x66DD5829U, 0x37524836U, 0x07D747C3U,
    0x944C6808U, 0xA4C967FDU, 0xF54677E2U, 0xC5C37817U, 0x43CE422BU, 0x734B4DDEU, 0x22C45DC1U, 0x12415234U, 0x81DA7DFFU,
    0xB15F720AU, 0xE0D06215U, 0xD0556DE0U, 0x7D747C32U, 0x4DF173C7U, 0x1C7E63D8U, 0x2CFB6C2DU, 0xBF6043E6U, 0x8FE54C13U,
    0xDE6A5C0CU, 0xEEEF53F9U, 0x68E269C5U, 0x58676630U, 0x09E8762FU, 0x396D79DAU, 0xAAF65611U, 0x9A7359E4U, 0xCBFC49FBU,
    0xFB79460EU, 0xACB0AFB8U, 0x9C35A04DU, 0xCDBAB052U, 0xFD3FBFA7U, 0x6EA4906CU, 0x5E219F99U, 0x0FAE8F86U, 0x3F2B8073U,
    0xB926BA4FU, 0x89A3B5BAU, 0xD82CA5A5U, 0xE8A9AA50U, 0x7B32859BU, 0x4BB78A6EU, 0x1A389A71U, 0x2ABD9584U, 0x879C8456U,
    0xB7198BA3U, 0xE6969BBCU, 0xD6139449U, 0x4588BB82U, 0x750DB477U, 0x2482A468U, 0x1407AB9DU, 0x920A91A1U, 0xA28F9E54U,
    0xF3008E4BU, 0xC38581BEU, 0x501EAE75U, 0x609BA180U, 0x3114B19FU, 0x0191BE6AU, 0xFAE8F864U, 0xCA6DF791U, 0x9BE2E78EU,
    0xAB67E87BU, 0x38FCC7B0U, 0x0879C845U, 0x59F6D85AU, 0x6973D7AFU, 0xEF7EED93U, 0xDFFBE266U, 0x8E74F279U, 0xBEF1FD8CU,
    0x2D6AD247U, 0x1DEFDDB2U, 0x4C60CDADU, 0x7CE5C258U, 0xD1C4D38AU, 0xE141DC7FU, 0xB0CECC60U, 0x804BC395U, 0x13D0EC5EU,
    0x2355E3ABU, 0x72DAF3B4U, 0x425FFC41U, 0xC452C67DU, 0xF4D7C988U, 0xA558D997U, 0x95DDD662U, 0x0646F9A9U, 0x36C3F65CU,
    0x674CE643U, 0x57C9E9B6U, 0xC8DF352FU, 0xF85A3ADAU, 0xA9D52AC5U, 0x99502530U, 0x0ACB0AFBU, 0x3A4E050EU, 0x6BC11511U,
    0x5B441AE4U, 0xDD4920D8U, 0xEDCC2F2DU, 0xBC433F32U, 0x8CC630C7U, 0x1F5D1F0CU, 0x2FD810F9U, 0x7E5700E6U, 0x4ED20F13U,
    0xE3F31EC1U, 0xD3761134U, 0x82F9012BU, 0xB27C0EDEU, 0x21E72115U, 0x11622EE0U, 0x40ED3EFFU, 0x7068310AU, 0xF6650B36U,
    0xC6E004C3U, 0x976F14DCU, 0xA7EA1B29U, 0x347134E2U, 0x04F43B17U, 0x557B2B08U, 0x65FE24FDU, 0x9E8762F3U, 0xAE026D06U,
    0xFF8D7D19U, 0xCF0872ECU, 0x5C935D27U, 0x6C1652D2U, 0x3D9942CDU, 0x0D1C4D38U, 0x8B117704U, 0xBB9478F1U, 0xEA1B68EEU,
    0xDA9E671BU, 0x490548D0U, 0x79804725U, 0x280F573AU, 0x188A58CFU, 0xB5AB491DU, 0x852E46E8U, 0xD4A156F7U, 0xE4245902U,
    0x77BF76C9U, 0x473A793CU, 0x16B56923U, 0x263066D6U, 0xA03D5CEAU, 0x90B8531FU, 0xC1374300U, 0xF1B24CF5U, 0x6229633EU,
    0x52AC6CCBU, 0x03237CD4U, 0x33A67321U, 0x646F9A97U, 0x54EA9562U, 0x0565857DU, 0x35E08A88U, 0xA67BA543U, 0x96FEAAB6U,
    0xC771BAA9U, 0xF7F4B55CU, 0x71F98F60U, 0x417C8095U, 0x10F3908AU, 0x20769F7FU, 0xB3EDB0B4U, 0x8368BF41U, 0xD2E7AF5EU,
    0xE262A0ABU, 0x4F43B179U, 0x7FC6BE8CU, 0x2E49AE93U, 0x1ECCA166U, 0x8D578EADU, 0xBDD28158U, 0xEC5D9147U, 0xDCD89EB2U,
    0x5AD5A48EU, 0x6A50AB7BU, 0x3BDFBB64U, 0x0B5AB491U, 0x98C19B5AU, 0xA84494AFU, 0xF9CB84B0U, 0xC94E8B45U, 0x3237CD4BU,
    0x02B2C2BEU, 0x533DD2A1U, 0x63B8DD54U, 0xF023F29FU, 0xC0A6FD6AU, 0x9129ED75U, 0xA1ACE280U, 0x27A1D8BCU, 0x1724D749U,
    0x46ABC756U, 0x762EC8A3U, 0xE5B5E768U, 0xD530E89DU, 0x84BFF882U, 0xB43AF777U, 0x191BE6A5U, 0x299EE950U, 0x7811F94FU,
    0x4894F6BAU, 0xDB0FD971U, 0xEB8AD684U, 0xBA05C69BU, 0x8A80C96EU, 0x0C8DF352U, 0x3C08FCA7U, 0x6D87ECB8U, 0x5D02E34DU,
    0xCE99CC86U, 0xFE1CC373U, 0xAF93D36CU, 0x9F16DC99U};

/**
 * \brief Look-up table for CRC-64 with polynomial 0x42F0E1EBA9EA3693.
 * reflected = true
 */
constexpr std::array<std::uint64_t, 256U> kLookUpTableCrc64{
    0x0000000000000000U, 0xB32E4CBE03A75F6FU, 0xF4843657A840A05BU, 0x47AA7AE9ABE7FF34U, 0x7BD0C384FF8F5E33U,
    0xC8FE8F3AFC28015CU, 0x8F54F5D357CFFE68U, 0x3C7AB96D5468A107U, 0xF7A18709FF1EBC66U, 0x448FCBB7FCB9E309U,
    0x325B15E575E1C3DU,  0xB00BFDE054F94352U, 0x8C71448D0091E255U, 0x3F5F08330336BD3AU, 0x78F572DAA8D1420EU,
    0xCBDB3E64AB761D61U, 0x7D9BA13851336649U, 0xCEB5ED8652943926U, 0x891F976FF973C612U, 0x3A31DBD1FAD4997DU,
    0x64B62BCAEBC387AU,  0xB5652E02AD1B6715U, 0xF2CF54EB06FC9821U, 0x41E11855055BC74EU, 0x8A3A2631AE2DDA2FU,
    0x39146A8FAD8A8540U, 0x7EBE1066066D7A74U, 0xCD905CD805CA251BU, 0xF1EAE5B551A2841CU, 0x42C4A90B5205DB73U,
    0x56ED3E2F9E22447U,  0xB6409F5CFA457B28U, 0xFB374270A266CC92U, 0x48190ECEA1C193FDU, 0xFB374270A266CC9U,
    0xBC9D3899098133A6U, 0x80E781F45DE992A1U, 0x33C9CD4A5E4ECDCEU, 0x7463B7A3F5A932FAU, 0xC74DFB1DF60E6D95U,
    0xC96C5795D7870F4U,  0xBFB889C75EDF2F9BU, 0xF812F32EF538D0AFU, 0x4B3CBF90F69F8FC0U, 0x774606FDA2F72EC7U,
    0xC4684A43A15071A8U, 0x83C230AA0AB78E9CU, 0x30EC7C140910D1F3U, 0x86ACE348F355AADBU, 0x3582AFF6F0F2F5B4U,
    0x7228D51F5B150A80U, 0xC10699A158B255EFU, 0xFD7C20CC0CDAF4E8U, 0x4E526C720F7DAB87U, 0x9F8169BA49A54B3U,
    0xBAD65A25A73D0BDCU, 0x710D64410C4B16BDU, 0xC22328FF0FEC49D2U, 0x85895216A40BB6E6U, 0x36A71EA8A7ACE989U,
    0xADDA7C5F3C4488EU,  0xB9F3EB7BF06317E1U, 0xFE5991925B84E8D5U, 0x4D77DD2C5823B7BAU, 0x64B62BCAEBC387A1U,
    0xD7986774E864D8CEU, 0x90321D9D438327FAU, 0x231C512340247895U, 0x1F66E84E144CD992U, 0xAC48A4F017EB86FDU,
    0xEBE2DE19BC0C79C9U, 0x58CC92A7BFAB26A6U, 0x9317ACC314DD3BC7U, 0x2039E07D177A64A8U, 0x67939A94BC9D9B9CU,
    0xD4BDD62ABF3AC4F3U, 0xE8C76F47EB5265F4U, 0x5BE923F9E8F53A9BU, 0x1C4359104312C5AFU, 0xAF6D15AE40B59AC0U,
    0x192D8AF2BAF0E1E8U, 0xAA03C64CB957BE87U, 0xEDA9BCA512B041B3U, 0x5E87F01B11171EDCU, 0x62FD4976457FBFDBU,
    0xD1D305C846D8E0B4U, 0x96797F21ED3F1F80U, 0x2557339FEE9840EFU, 0xEE8C0DFB45EE5D8EU, 0x5DA24145464902E1U,
    0x1A083BACEDAEFDD5U, 0xA9267712EE09A2BAU, 0x955CCE7FBA6103BDU, 0x267282C1B9C65CD2U, 0x61D8F8281221A3E6U,
    0xD2F6B4961186FC89U, 0x9F8169BA49A54B33U, 0x2CAF25044A02145CU, 0x6B055FEDE1E5EB68U, 0xD82B1353E242B407U,
    0xE451AA3EB62A1500U, 0x577FE680B58D4A6FU, 0x10D59C691E6AB55BU, 0xA3FBD0D71DCDEA34U, 0x6820EEB3B6BBF755U,
    0xDB0EA20DB51CA83AU, 0x9CA4D8E41EFB570EU, 0x2F8A945A1D5C0861U, 0x13F02D374934A966U, 0xA0DE61894A93F609U,
    0xE7741B60E174093DU, 0x545A57DEE2D35652U, 0xE21AC88218962D7AU, 0x5134843C1B317215U, 0x169EFED5B0D68D21U,
    0xA5B0B26BB371D24EU, 0x99CA0B06E7197349U, 0x2AE447B8E4BE2C26U, 0x6D4E3D514F59D312U, 0xDE6071EF4CFE8C7DU,
    0x15BB4F8BE788911CU, 0xA6950335E42FCE73U, 0xE13F79DC4FC83147U, 0x521135624C6F6E28U, 0x6E6B8C0F1807CF2FU,
    0xDD45C0B11BA09040U, 0x9AEFBA58B0476F74U, 0x29C1F6E6B3E0301BU, 0xC96C5795D7870F42U, 0x7A421B2BD420502DU,
    0x3DE861C27FC7AF19U, 0x8EC62D7C7C60F076U, 0xB2BC941128085171U, 0x192D8AF2BAF0E1EU,  0x4638A2468048F12AU,
    0xF516EEF883EFAE45U, 0x3ECDD09C2899B324U, 0x8DE39C222B3EEC4BU, 0xCA49E6CB80D9137FU, 0x7967AA75837E4C10U,
    0x451D1318D716ED17U, 0xF6335FA6D4B1B278U, 0xB199254F7F564D4CU, 0x2B769F17CF11223U,  0xB4F7F6AD86B4690BU,
    0x7D9BA1385133664U,  0x4073C0FA2EF4C950U, 0xF35D8C442D53963FU, 0xCF273529793B3738U, 0x7C0979977A9C6857U,
    0x3BA3037ED17B9763U, 0x888D4FC0D2DCC80CU, 0x435671A479AAD56DU, 0xF0783D1A7A0D8A02U, 0xB7D247F3D1EA7536U,
    0x4FC0B4DD24D2A59U,  0x3886B22086258B5EU, 0x8BA8FE9E8582D431U, 0xCC0284772E652B05U, 0x7F2CC8C92DC2746AU,
    0x325B15E575E1C3D0U, 0x8175595B76469CBFU, 0xC6DF23B2DDA1638BU, 0x75F16F0CDE063CE4U, 0x498BD6618A6E9DE3U,
    0xFAA59ADF89C9C28CU, 0xBD0FE036222E3DB8U, 0xE21AC88218962D7U,  0xC5FA92EC8AFF7FB6U, 0x76D4DE52895820D9U,
    0x317EA4BB22BFDFEDU, 0x8250E80521188082U, 0xBE2A516875702185U, 0xD041DD676D77EEAU,  0x4AAE673FDD3081DEU,
    0xF9802B81DE97DEB1U, 0x4FC0B4DD24D2A599U, 0xFCEEF8632775FAF6U, 0xBB44828A8C9205C2U, 0x86ACE348F355AADU,
    0x34107759DB5DFBAAU, 0x873E3BE7D8FAA4C5U, 0xC094410E731D5BF1U, 0x73BA0DB070BA049EU, 0xB86133D4DBCC19FFU,
    0xB4F7F6AD86B4690U,  0x4CE50583738CB9A4U, 0xFFCB493D702BE6CBU, 0xC3B1F050244347CCU, 0x709FBCEE27E418A3U,
    0x3735C6078C03E797U, 0x841B8AB98FA4B8F8U, 0xADDA7C5F3C4488E3U, 0x1EF430E13FE3D78CU, 0x595E4A08940428B8U,
    0xEA7006B697A377D7U, 0xD60ABFDBC3CBD6D0U, 0x6524F365C06C89BFU, 0x228E898C6B8B768BU, 0x91A0C532682C29E4U,
    0x5A7BFB56C35A3485U, 0xE955B7E8C0FD6BEAU, 0xAEFFCD016B1A94DEU, 0x1DD181BF68BDCBB1U, 0x21AB38D23CD56AB6U,
    0x9285746C3F7235D9U, 0xD52F0E859495CAEDU, 0x6601423B97329582U, 0xD041DD676D77EEAAU, 0x636F91D96ED0B1C5U,
    0x24C5EB30C5374EF1U, 0x97EBA78EC690119EU, 0xAB911EE392F8B099U, 0x18BF525D915FEFF6U, 0x5F1528B43AB810C2U,
    0xEC3B640A391F4FADU, 0x27E05A6E926952CCU, 0x94CE16D091CE0DA3U, 0xD3646C393A29F297U, 0x604A2087398EADF8U,
    0x5C3099EA6DE60CFFU, 0xEF1ED5546E415390U, 0xA8B4AFBDC5A6ACA4U, 0x1B9AE303C601F3CBU, 0x56ED3E2F9E224471U,
    0xE5C372919D851B1EU, 0xA26908783662E42AU, 0x114744C635C5BB45U, 0x2D3DFDAB61AD1A42U, 0x9E13B115620A452DU,
    0xD9B9CBFCC9EDBA19U, 0x6A978742CA4AE576U, 0xA14CB926613CF817U, 0x1262F598629BA778U, 0x55C88F71C97C584CU,
    0xE6E6C3CFCADB0723U, 0xDA9C7AA29EB3A624U, 0x69B2361C9D14F94BU, 0x2E184CF536F3067FU, 0x9D36004B35545910U,
    0x2B769F17CF112238U, 0x9858D3A9CCB67D57U, 0xDFF2A94067518263U, 0x6CDCE5FE64F6DD0CU, 0x50A65C93309E7C0BU,
    0xE388102D33392364U, 0xA4226AC498DEDC50U, 0x170C267A9B79833FU, 0xDCD7181E300F9E5EU, 0x6FF954A033A8C131U,
    0x28532E49984F3E05U, 0x9B7D62F79BE8616AU, 0xA707DB9ACF80C06DU, 0x14299724CC279F02U, 0x5383EDCD67C06036U,
    0xE0ADA17364673F59U};

}  // namespace internal

std::uint32_t Crc::CalculateCRC32P04(const BufferView buffer_view, const std::uint32_t start_value,
                                     const bool is_first_call) noexcept {
  CRC32 crc{};

  if (is_first_call) {
    crc = kInitialValueCRC32P04;
  } else {
    crc = start_value ^ kXORValueCRC32P04;
  }

  for (const auto& byte : buffer_view) {
    const std::uint8_t pos = static_cast<std::uint8_t>(byte ^ crc);
    const std::uint32_t lookup = internal::kLookUpTableCrc32P04[pos];
    crc = lookup ^ (crc >> 8U);
  }

  crc ^= kXORValueCRC32P04;
  return crc;
}

std::uint64_t Crc::CalculateCRC64P07(const BufferView buffer_view, const CRC64 start_value,
                                     const bool is_first_call) noexcept {
  CRC64 crc{};

  if (is_first_call) {
    crc = kInitialValueCRC64;
  } else {
    crc = start_value ^ kXORValueCRC64;
  }

  for (const auto& byte : buffer_view) {
    const std::uint8_t index = static_cast<std::uint8_t>(byte ^ crc);
    const std::uint64_t lookup = internal::kLookUpTableCrc64[index];
    crc >>= 8ULL;
    crc ^= lookup;
  }

  crc ^= kXORValueCRC64;
  return crc;
}

}  // namespace crc
}  // namespace ara
