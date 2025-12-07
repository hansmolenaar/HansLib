#include "UndirectedGraphFromG6.h"
#include "MyException.h"
#include <iostream>
#include <set>

using namespace Graph;

using SortedEdge = std::pair<Vertex, Vertex>;

namespace
{

std::vector<std::string> ListNumVertices_3 = {"Bw", "BW", "BG", "B?"};

std::vector<std::string> ListNumVertices_4 = {"C^", "C?", "C~", "C@", "CF", "C`", "CR", "CJ", "CN", "Cr", "CB"};

std::vector<std::string> ListNumVertices_5_connected = {
    "D@s", "DBw", "DD[", "DFw", "DF{", "DDW", "D`[", "DqK", "DJ{", "Dr[", "Dr{",
    "D^{", "D~{", "D?{", "D`{", "DJk", "DN{", "D@{", "DB{", "DR{", "Dd[",
};

std::vector<std::string> ListNumVertices_5 = {
    "D@s", "D??", "DIK", "DBw", "DD[", "DFw", "DF{", "DDW", "D?C", "D`K", "D`[", "DqK",
    "DJ[", "DJ{", "Dr[", "Dr{", "D^{", "D~{", "D?{", "DGC", "D@K", "D`{", "DJk", "DN{",
    "D?K", "D?[", "D@o", "D@{", "DB{", "DR{", "Dd[", "DAK", "D@[", "DB[",
};

std::vector<std::string> ListNumVertices_6_connected = {
    "Es\\o", "EF~w", "E`ow", "EqNw",  "E?Fg",  "EJ^w",  "E}lw", "E^~w", "E~~w",  "E?NO",  "E?dg",  "E?No", "E?lo",
    "E?~w",  "EAIW", "EAN_", "ECSw",  "E@hO",  "E@po",  "E?Bw", "E@Nw", "EoSo",  "EiKw",  "Es\\w", "EFzw", "E`NW",
    "E`Lw",  "E`Nw", "E`~o", "E`~w",  "E{Sw",  "ER~w",  "EJ]w", "EJ~w", "EN~w",  "Et\\w", "Er~w",  "E?^o", "E?lw",
    "E?~o",  "EGEw", "EANw", "ECXo",  "E@ow",  "E@ro",  "E_Lw", "E_lo", "EC\\o", "E@lo",  "ES\\o", "E@hW", "EKSw",
    "E`dg",  "E?Nw", "E`lo", "EQlw",  "E`\\w", "E@rw",  "EGFw", "EC^w", "EINw",  "EDZw",  "EBzw",  "EELg", "EElw",
    "EMlw",  "ER~o", "EqLw", "ET\\w", "Ed^w",  "EB^w",  "EB~w", "EFxw", "ER^w",  "E@~w",  "Er^w",  "E_lw", "Eqlw",
    "EGcw",  "EBxw", "EDZW", "ETXW",  "EANg",  "EoSw",  "ED^_", "E@pw", "E@^w",  "EImw",  "EHuw",  "E@~o", "E`^o",
    "EBzo",  "EAMw", "EPTw", "EAlw",  "EPVW",  "Er\\w", "ED^w", "E`^w", "Ed\\w", "ED\\w", "E`lw",  "E@lw", "EC\\w",
    "EIMw",  "E@^o", "E@vo", "E?^w",  "E@NW",  "E`LW",  "E?Fw", "E?NW"};

std::vector<std::string> ListNumVertices_6 = {
    "E?CW",  "E???",  "Es\\o", "EF~w", "EwCW",  "E`ow",  "EqNw",  "E?Fg",  "EJ\\w", "EJ^w", "E}lw",  "E^~w",  "E~~w",
    "E@GW",  "E?NO",  "E??G",  "E?dg", "E?No",  "E?lo",  "E?~w",  "E`?G",  "EAIW",  "EIGW", "EAN_",  "ECSw",  "E@hO",
    "E@po",  "E?Bw",  "E@Kw",  "E@Nw", "EoSo",  "EiKw",  "Es\\w", "EFzw",  "E`NW",  "E`Lw", "E`Nw",  "E`~o",  "E`~w",
    "E{Sw",  "ER~w",  "EJ]w",  "EJ~w", "EN~w",  "Et\\w", "Er~w",  "E?d_",  "E?\\o", "E?^o", "E??W",  "E?lw",  "E?~o",
    "E??w",  "EGCw",  "EGEw",  "E?@w", "EANw",  "ECXo",  "E@ow",  "E@ro",  "E_Lw",  "E@?G", "E_lo",  "EC\\o", "E@lo",
    "ES\\o", "E@hW",  "EKSw",  "E`dg", "E?Nw",  "E`lo",  "EQlw",  "E`\\w", "E@rw",  "EGFw", "EC^w",  "EINw",  "EDZw",
    "EBzw",  "EELg",  "EElw",  "EMlw", "ER~o",  "EqLw",  "ET\\w", "Ed^w",  "EB^w",  "EB~w", "EFxw",  "ER^w",  "E@~w",
    "Er^w",  "E_lw",  "Eqlw",  "EGcw", "EBxw",  "EDZW",  "ETXW",  "EANg",  "EoSw",  "ED^_", "E@pw",  "E?Lw",  "E@^w",
    "EImw",  "EHuw",  "E@~o",  "E`^o", "EBzo",  "EAMw",  "EPTw",  "EAlw",  "EPVW",  "E_GW", "EGEW",  "Er\\w", "ED^w",
    "E`^w",  "Ed\\w", "ED\\w", "E`lw", "E@lw",  "EC\\w", "EIMw",  "E@^o",  "E@vo",  "E?^w", "E@NW",  "E`LW",  "E?Fw",
    "E?NW",  "E?D_",  "EGCW",  "E?So", "E?Cw",  "E?Dg",  "E?F_",  "E@N?",  "E`GW",  "E?Dw", "E?Kw",  "E?Sw",  "E?Lo",
    "E@LW",  "EGDw",  "EGSw",  "E_Kw", "E?\\w", "E`Kw",  "EALw",  "EIKw",  "E@Lw",  "EILw", "E@\\w", "EB\\w"};

std::vector<std::string> ListNumVertices_7_connected = {
    "FqN~w",  "FJ\\~w", "F}lzw",  "F}l~w", "F^~~w",  "F~~~w",  "F?C^w",  "F?O|_",  "F??Ng", "F?Sv?",  "F?dXw",
    "F??^G",  "F??^o",  "F?~v_",  "F?~~w", "F@IQO",  "FCOpW",  "F?AZO",  "FoDPO",  "FCS~?", "F@MYw",  "F?B~w",
    "F@K~w",  "F?GUW",  "F??Fw",  "F?G]_", "FWEYo",  "FF~vW",  "FF~~w",  "F`N@w",  "F`o~_", "F}hXw",  "FJ\\|w",
    "F?_ZG",  "FJ^~w",  "FJ~~w",  "FN~~w", "Fj]|w",  "Fr~~w",  "F?Ddo",  "F?C^G",  "F?C~?", "F??Nw",  "F?db_",
    "F?KuG",  "F?org",  "F@IYo",  "Fo@Xo", "F?S~w",  "F??^O",  "F?\\v_", "FIQ|o",  "FIQ|w", "F?~vg",  "FCO_w",
    "F_GZ_",  "F@OsW",  "F@`J_",  "FCDHW", "F`BHo",  "F`BHw",  "F??}w",  "F?Azo",  "F_Oxo", "F`GZw",  "FI_xo",
    "F?B~o",  "F`IYw",  "FYCXW",  "F_K~_", "F_K~w",  "FoDzo",  "F@pHg",  "FWCYw",  "FqOxo", "F@~vg",  "F@~v_",
    "Fs\\vw", "F}oxw",  "F?G^w",  "FTPHw", "FwCZw",  "FwC^w",  "F`o~w",  "F`Kyw",  "F`Kzw", "F`K~w",  "F`N~w",
    "F?HSo",  "F{dzw",  "F`~~w",  "FAL~o", "F_GZw",  "FAN\\w", "FwdXw",  "FBqng",  "FsXXw", "F`AZO",  "FqN~o",
    "FJz\\w", "FEl~w",  "F`luW",  "FQl~_", "FqhXw",  "FdlrW",  "Ft\\~w", "FQMiw",  "FIiZw", "F@pro",  "FGE^w",
    "F`N^w",  "FTX^w",  "F`lvw",  "FhN^w", "FQl~w",  "F`~vw",  "FqL~w",  "F{S~w",  "Fql~w", "FMl~w",  "FR~vw",
    "F@?Nw",  "F?Dfw",  "F?O~w",  "F?Ffw", "F?`~w",  "F@G^w",  "F?Svw",  "F?dfw",  "F?Lvw", "F@FNw",  "F?dnw",
    "F?Nvw",  "F?\\vw", "F?lvw",  "F?^vw", "F?~vw",  "F`?Nw",  "F_G^w",  "FAI^w",  "F@hVw", "F@h^w",  "F`G^w",
    "F_Lvw",  "FC\\vw", "FoSvw",  "F`dnw", "FS\\vw", "F@IQW",  "F?StW",  "F_G^_",  "F?ur_", "Fhuzw",  "FSXPw",
    "F?O|o",  "FqL~o",  "F?df_",  "FAG[w", "F`dn_",  "F_Azw",  "F?Lvo",  "F?S~_",  "F?o~g", "F@Fmw",  "FAIZw",
    "FD^vO",  "FAG^G",  "F@IYw",  "FBFLW", "F?luW",  "F?luw",  "F?l~_",  "F?l~g",  "F?uzw", "F?~rw",  "F`AZW",
    "FAI^o",  "FALcw",  "FIG\\w", "FIG^w", "FANLg",  "F@h^g",  "FAYto",  "FANfo",  "FANcw", "FCS~W",  "FIJ\\o",
    "FIJ\\w", "FCXvo",  "F_}rg",  "FCx~g", "FGuzw",  "F@U~O",  "FC~rw",  "FBZ\\w", "FQMZW", "FQo~g",  "FEhzo",
    "FDhzo",  "FBzvo",  "Fqdhw",  "FMhXw", "FDxZg",  "FXd]w",  "FFhmw",  "FD^fw",  "Fdhzo", "FqLzo",  "FCS~w",
    "FEl~W",  "FqSpW",  "FKl~g",  "FKuzw", "FR~~w",  "FCUj_",  "F?Mzo",  "F_Lpw",  "F@Miw", "FDLiw",  "F`XXw",
    "FDlrW",  "FJW{w",  "FoS~_",  "FANTW", "FDZ^w",  "FAl~w",  "F?dn_",  "F?AZW",  "FoDPW", "FK`Xo",  "FQG]W",
    "FDZ~w",  "F?L~w",  "F@v~w",  "FGdtw", "FCLmw",  "F?qzw",  "FAJ\\w", "F@NMw",  "Fq?gw", "F@rvw",  "F@Nmo",
    "FwDXo",  "Folqw",  "FQluW",  "FIm~w", "FHu~w",  "F@~vw",  "FiM~w",  "Fs\\zw", "Fqlzw", "FT\\}w", "FD^~o",
    "Fimzw",  "F`~rw",  "F@J]o",  "F?lv_", "F@O\\G", "F@oZG",  "FRh]w",  "FgEzo",  "FqHXw", "F`NJg",  "F@RHw",
    "F\\^]w", "Fr^~w",  "F?SsW",
};

std::vector<std::string> ListNumVertices_7 = {
    "FqN~w",  "FJ\\zw", "FJ\\~w", "F}lzw",  "F}l~w",  "F^~~w",  "F~~~w",  "F????",  "F??GW", "F?CWw", "F?C^w", "F?O|_",
    "F??Ng",  "FI?go",  "F?Sv?",  "F???G",  "F?COW",  "F?dXw",  "F??^G",  "F??^o",  "F?~v_", "F?~~w", "F`?GW", "F@IQO",
    "FCOpW",  "F?AZO",  "FoDPO",  "FCS~?",  "F@MYw",  "F?B~w",  "FGC?G",  "F@Kxw",  "F@K~w", "F?GUW", "F??Fw", "F?G]_",
    "FWEYo",  "FF~vW",  "FF~~w",  "FwCWw",  "F`N@w",  "F`o~_",  "F}hXw",  "FJ\\|w", "F?_ZG", "FJ^~w", "FJ~~w", "FN~~w",
    "Fj]|w",  "Fr~~w",  "F?Ddo",  "F?C^?",  "F?C^G",  "F?C~?",  "F??Nw",  "F@G]?",  "F?db_", "F?KuG", "F?org", "F@IYo",
    "Fo@Xo",  "F??^?",  "F?S~w",  "F??^O",  "F???W",  "FIOxo",  "F?\\v_", "FIQ|o",  "FIQ|w", "F?~vg", "FCO_w", "F_GZ_",
    "F@OsW",  "F@`J_",  "FCDHW",  "FJ?GW",  "F??xo",  "F?@Xo",  "F???w",  "F`BHo",  "F`BHw", "FGDzo", "F??}w", "F?@zo",
    "F??@w",  "F?Azo",  "F_Oxo",  "F`GZw",  "FI_xo",  "F?B~o",  "F`IYw",  "FYCXW",  "F??Bw", "F_K~_", "F_K~w", "FoDzo",
    "FwCOW",  "F@pHg",  "FWCYw",  "FqOxo",  "F@~vg",  "F?C?G",  "F@~v_",  "Fs\\vw", "F}oxw", "F?G^w", "FTPHw", "FwCZw",
    "FwC^w",  "F`o~w",  "F`Kyw",  "F`Kzw",  "F`K~w",  "F`N~w",  "F?HSo",  "F{dzw",  "F`~~w", "FAL~o", "F_GZw", "FAN\\w",
    "FwdXw",  "FBqng",  "FsXXw",  "F`AZO",  "FqN~o",  "FJz\\w", "FEl~w",  "F@?M?",  "F`luW", "FQl~_", "FqhXw", "FdlrW",
    "Ft\\~w", "FQMiw",  "FIiZw",  "F@pro",  "FGE^w",  "F`N^w",  "FTX^w",  "F`lvw",  "FhN^w", "FQl~w", "F`~vw", "FqL~w",
    "F{S~w",  "Fql~w",  "FMl~w",  "FR~vw",  "F@?Nw",  "F?Dfw",  "F?O~w",  "F?Ffw",  "F?`~w", "F@G^w", "F?Svw", "F?dfw",
    "F?Lvw",  "F@FNw",  "F?dnw",  "F?Nvw",  "F?\\vw", "F?lvw",  "F?^vw",  "F?~vw",  "F`?Nw", "F_G^w", "FAI^w", "F@hVw",
    "F@h^w",  "F`G^w",  "F_Lvw",  "FC\\vw", "FoSvw",  "F`dnw",  "FS\\vw", "F@IQW",  "F?StW", "F_G^_", "F?ur_", "F??H_",
    "F`?Gw",  "Fhuzw",  "FSXPw",  "F?O|o",  "FqL~o",  "F?df_",  "FAG[w",  "F`dn_",  "F_Azw", "F?Lvo", "F?S~_", "F?o~g",
    "F@Fmw",  "FAIZw",  "FD^vO",  "FAG^G",  "F@IYw",  "FBFLW",  "F?luW",  "F?luw",  "F?l~_", "F?l~g", "F?uzw", "F?~rw",
    "F`AZW",  "FAI^o",  "FALcw",  "FIG\\w", "FIG^w",  "FANLg",  "F@h^g",  "FAYto",  "FANfo", "FANcw", "FCS~W", "FIJ\\o",
    "FIJ\\w", "FCXvo",  "F_}rg",  "FCx~g",  "FGuzw",  "F@U~O",  "FC~rw",  "FBZ\\w", "FQMZW", "FQo~g", "FEhzo", "FDhzo",
    "FBzvo",  "Fqdhw",  "FMhXw",  "FDxZg",  "FXd]w",  "FFhmw",  "FD^fw",  "Fdhzo",  "FqLzo", "FCS~w", "FEl~W", "FqSpW",
    "FKl~g",  "FKuzw",  "FR~~w",  "FCUj_",  "F??OW",  "F?Mzo",  "F_Lpw",  "F@Miw",  "FDLiw", "F`XXw", "FDlrW", "FJW{w",
    "FoS~_",  "FANTW",  "FDZ^w",  "FAl~w",  "F?dn_",  "F?AZW",  "FoDPW",  "FK`Xo",  "FQG]W", "FDZ~w", "F?Czw", "F?L~w",
    "F@v~w",  "FGdtw",  "FCLmw",  "F?qzw",  "FAJ\\w", "F@NMw",  "Fq?gw",  "F@rvw",  "F@Nmo", "FwDXo", "Folqw", "FQluW",
    "FIm~w",  "FHu~w",  "F@~vw",  "FiM~w",  "Fs\\zw", "Fqlzw",  "FT\\}w", "FD^~o",  "Fimzw", "F`~rw", "F@J]o", "F?lv_",
    "F@O\\G", "F@oZG",  "FRh]w",  "F@@Gw",  "FgEzo",  "FqHXw",  "F`NJg",  "F?CeG",  "FG?[o", "FGC[w", "F@RHw", "F\\^]w",
    "Fr^~w",  "F?SsW",
};

std::vector<std::string> ListNumVertices_8_connected = {
    "G@Kx~{",  "G?_gqK",  "Gs\\v~w", "GF~~~{",  "GwC^?{",  "G}hPW{",  "GqN~~{",  "GJ\\z~{", "G~z\\z{", "G^~~~{",
    "G~~~~{",  "G?DPV?",  "G?DPTC",  "G?`HW{",  "G@GQSG",  "G??MPg",  "G???Ns",  "G?CW~{",  "GoCQPG",  "G?CHM_",
    "G??gv_",  "G?@HcK",  "G???^w",  "GsP@xw",  "G?@Ht_",  "Gs`zro",  "G?~~~{",  "G`?GZc",  "G??CZc",  "G`_gqK",
    "GsOiho",  "G???F{",  "G??F~{",  "G?B~~{",  "G?O_c[",  "G?GQSK",  "G?zTjo",  "Gs@ipo",  "G`dnfw",  "GQo~nw",
    "G?z\\z{", "G?xPcK",  "G@hRSk",  "GoSv~w",  "GIiZ~w",  "G@lv]{",  "GsOax{",  "GANLfK",  "G{O_w{",  "GANcvK",
    "GCS~VK",  "G_h\\rk", "GoDPV{",  "GqGTY{",  "G`deX{",  "Gj?HWw",  "G?DP^{",  "G?`@pk",  "G?XX~{",  "G?S~^{",
    "G?qz~{",  "GJ?G^{",  "GGD_~{",  "GGC^F{",  "GGDc~{",  "GGCZ^{",  "GGC^^{",  "GASp^{",  "GGDXv{",  "G@O}v{",
    "GHCZ^{",  "GGD\\v{", "GAHX~{",  "GHC^^{",  "G@P\\~{", "G@pHn{",  "G`BHv{",  "G_C~V{",  "GKCZ^{",  "GGEZ^{",
    "G`BH~{",  "G`C^^{",  "G@bJ~{",  "GAJ\\v{", "GGEzv{",  "G_Ezv{",  "G@`~v{",  "GAJ\\~{", "GGDzv{",  "GGD~v{",
    "GGF~v{",  "GGSs~{",  "GIGX~{",  "GIG\\~{", "GPLQ^{",  "GJOg~{",  "GIHX~{",  "GGS~f{",  "G@XX~{",  "GAXt~{",
    "G@X\\~{", "G@fbv{",  "GiGX~{",  "GIIX~{",  "GII[~{",  "GiG\\~{", "GD`j~{",  "G@jR~{",  "GXCY^{",  "GHFH~{",
    "G@S~^{",  "GCS~F{",  "GANc~{",  "GCS~^{",  "G@U~V{",  "GAwzn{",  "GCL~V{",  "GIJ\\v{", "GAlnn{",  "GIJ\\~{",
    "GDSz^{",  "GCUzv{",  "G@]^n{",  "G@Z\\~{", "GALzv{",  "GAL~v{",  "GAN~v{",  "GDYR^{",  "GhCZ^{",  "G@^Dn{",
    "GhC^^{",  "GBaj~{",  "GCUrV{",  "G@UvV{",  "GAMjn{",  "GANT^{",  "GHFK~{",  "GPOy~{",  "GDPk~{",  "GCSz^{",
    "G@oy~{",  "G@hu~{",  "GB_~^{",  "GCLm~{",  "GGeZf{",  "G_K~f{",  "G`FH~{",  "G_ox~{",  "G_K~n{",  "G@Nev{",
    "G@NU^{",  "G@NJn{",  "G@Ne~{",  "GCdj~{",  "G@rvv{",  "G_Nrv{",  "GC]r^{",  "G_Mzv{",  "G_[~n{",  "G_Nr~{",
    "GgEzv{",  "G@pzv{",  "G@lu^{",  "G@p~v{",  "G@r~v{",  "Gw?W~{",  "G@ou^{",  "GEGi~{",  "GEGm~{",  "G@Nmv{",
    "G@Nm~{",  "GpHY~{",  "G@vfn{",  "G_lr~{",  "GFY^^{",  "GCx~n{",  "GC|rn{",  "GC^r~{",  "GDxZn{",  "G@^u~{",
    "G_}rn{",  "GC~r~{",  "GiIX~{",  "GDdj^{",  "GHpX~{",  "GDLm^{",  "GJQk~{",  "GIdl~{",  "GBiZ~{",  "GAlv^{",
    "GA\\t~{", "G@lu~{",  "GMhX~{",  "GRW}~{",  "GBZ\\~{", "GRWy~{",  "GJXX~{",  "GIL~v{",  "GIN~v{",  "GINT^{",
    "G`XX~{",  "GBpl~{",  "G`X\\~{", "GqSp^{",  "GoS~f{",  "GqHX~{",  "GhFH~{",  "GaMjn{",  "GIMk~{",  "G`Vd~{",
    "GQlu^{",  "GdSz^{",  "GKszn{",  "GbM^^{",  "GQqz~{",  "GDlr^{",  "GP\\^n{", "Golq~{",  "GElr^{",  "GEhzv{",
    "GDZ^v{",  "GDZ~v{",  "GRX\\~{", "Gq\\t~{", "GS\\~f{", "GIm~n{",  "GI~t~{",  "GJZ\\~{", "GdX\\~{", "GD^vV{",
    "GD^v^{",  "G}ox~{",  "G]qz~{",  "GF~v^{",  "GTPH~{",  "G`S~^{",  "GwDXv{",  "G`NJn{",  "GQMi~{",  "GKdzv{",
    "Gqox~{",  "GqLzv{",  "G??ZCS",  "G?hTrk",  "G`?KzW",  "GKS~\\{", "GGF~vo",  "G@hZCc",  "G?@Hdo",  "G?XPc[",
    "G???^g",  "GAI?g[",  "G?@PSo",  "G?Oc_w",  "G_D`Gs",  "G@`AXk",  "GHdu\\{", "GAh`ks",  "GCSXJC",  "G?qaho",
    "G@?La[",  "GqKsY[",  "G???~G",  "G?GQKo",  "G??_uK",  "GwC^~{",  "G~rHx{",  "GoSv~{",  "G?mzz{",  "G~`HW{",
    "GKdax{",  "G`?N~w",  "G`?Lz{",  "GICX|[",  "G?qix{",  "G??x}{",  "G`?MXw",  "G@`@W{",  "G??Dzw",  "G??Cz[",
    "GEChX[",  "GIi^bw",  "GSXTz{",  "G@G\\a[", "GtPHOk",  "Gv?IXW",  "Gvzax{",  "G?r@pg",  "GSP@xw",  "G?DPS{",
    "GAG]|{",  "GBOk~W",  "G`G\\zw", "G_ltjs",  "G@lv]w",  "G@zTz{",  "G_]~bk",  "G@nRnS",  "GIM}v[",  "G`iZQk",
    "GSXP~w",  "GQMZ^k",  "GsXTzw",  "GINT^o",  "GBZT[{",  "GINe|{",  "GsXPW{",  "GqYPW{",  "Gr_YX[",  "GKoXzg",
    "GDxRK{",  "GSXZ`{",  "G]Og|{",  "G@luvG",  "GSXX~c",  "GSLi~k",  "GdZDzw",  "Guhax{",  "GgN\\rk", "G}hHg{",
    "GrddY{",  "GqMZj[",  "GRqZ^k",  "Gqlsz[",  "Gsxqx{",  "GMnax{",  "GP^uu{",  "GD^vU{",  "GEl~~w",  "GrotY{",
    "G`zTzw",  "GQ}rm[",  "GqluX{",  "Grqix{",  "GRl~Ms",  "G]`@W{",  "G?urf[",  "G?\\utk", "G@dTJS",  "G@MQ^?",
    "GGFLbo",  "GoDPZo",  "GoDP^o",  "GqGR[w",  "Gr`?x[",  "GqMAXk",  "G@h^vg",  "G@X^dw",  "GCS~~w",  "GANvS{",
    "GqMAh[",  "G_}ahk",  "G@h^e[",  "GBYk}{",  "GPpX~k",  "GWS}|{",  "G@zZrk",  "G@lu~w",  "GEhXrK",  "GEhax{",
    "GqGZ[{",  "GpDHzw",  "GgMP}w",  "G`hP~w",  "GRQH}w",  "GDZDzw",  "G`ddzw",  "GoSr~w",  "GDZDz{",  "G`ddz{",
    "GDZcy{",  "GqopW{",  "GoTXpk",  "GEhrS{",  "GMhP[{",  "GqSpZ{",  "G`h\\rg", "GK^@|k",  "G`Urt[",  "GqHX~o",
    "GD^Dj[",  "GDZUX{",  "G`X\\tk", "GBhmk{",  "GdZDz{",  "GpFjq{",  "GQqzvk",  "GJqix{",  "GJM]^[",  "GqJ\\z{",
    "GcL~v[",  "GDZ^vw",  "GMMZ\\[", "GJMk}[",  "GqNax{",  "GDYz~o",  "G?L?nC",  "Gq?@Ww",  "G@?]VK",  "G@GSY[",
    "GOBAx{",  "G@GR[{",  "G@?Z]{",  "G@CNM[",  "G?EJZw",  "G?G^]w",  "G?EVZ{",  "GoCAhW",  "GoCAh[",  "GA?X^O",
    "G@HCyw",  "G@BDY{",  "G@?kyw",  "G?Ss^{",  "GA?}TS",  "G@Aiu[",  "G?Eja{",  "G@@ku{",  "G?ErU{",  "G@Aiv{",
    "G?C}^k",  "G?Hszs",  "G?FRX{",  "G?Er]{",  "G_?|Qs",  "G_?ytK",  "G?O|fo",  "GGAXu[",  "G_Aix{",  "G?HXvk",
    "G?K]Nc",  "GCCZ^K",  "GC_Zj[",  "G@G^Uk",  "GGKQK[",  "GGDPS{",  "GAOpS{",  "GI?gt{",  "GI?gv{",  "GAHco{",
    "G@P_|s",  "GGHSsw",  "GGO\\dw", "G@IQ]{",  "G@DJ\\w", "G@Ok}w",  "G@EJZw",  "G@DdY{",  "G?XTtg",  "G?XTtk",
    "G?Sr^w",  "G?XP~k",  "G??guK",  "G@EIzw",  "G?UP~K",  "GCDH~[",  "G?Ss~[",  "GAG[~{",  "GAHe|{",  "G@Om|w",
    "GAG]|w",  "G?Sv^w",  "G@G]UK",  "G?KuMs",  "G@G[yw",  "GAKs][",  "G@IYu{",  "GAKuZ[",  "GAKu\\[", "GCKr]W",
    "GADl\\s", "G@G}m[",  "Go@Xz{",  "G@K]m[",  "G?S~Bc",  "GANDG{",  "GDHG~K",  "GCDhvK",  "G?czb[",  "G?L\\f[",
    "G?Yq|s",  "G?cznK",  "G?V`~s",  "G?@Htk",  "G?hZlk",  "G?h\\rk", "G?ozn{",  "G?LuZs",  "G?Mi~k",  "GAEj|w",
    "G?Mz~k",  "G?Mzv_",  "G?AYpo",  "G_GSZ_",  "G@`?xw",  "GCO_~[",  "GCO_~{",  "GCOaxw",  "GQ?g}K",  "G_l@Gk",
    "G`GSY[",  "G??q\\s", "G?CTZW",  "GGCmc[",  "GCDH^K",  "GCDH^[",  "G`GR[{",  "GG_q|{",  "Go?Ax{",  "G??i|s",
    "G@dTZ[",  "GGgq{{",  "GAcr\\{", "G@UJh{",  "GCOzs{",  "G@Qi|s",  "GAJax{",  "G@P\\^k", "GAIZ~w",  "GAMPY[",
    "GAcpZ[",  "GAHXvK",  "GAHkzs",  "GOPXvk",  "G?BDz{",  "G@O|}w",  "GQAzu[",  "GAJ\\r{", "G_hP_[",  "GCUb?{",
    "Gp@Gzo",  "GoDPZ{",  "GaIHg{",  "GGUTX{",  "G?Baps",  "G??X~G",  "G@hSz[",  "GqGUXw",  "GGMP}w",  "G@hP~w",
    "GaClz{",  "GBIM~w",  "G?AZRk",  "G?AZVk",  "GALel[",  "G@TtU[",  "GIEht{",  "GAYXzk",  "G@Xs}{",  "GG]Pm[",
    "G@h[z[",  "GILe|{",  "GGS}|{",  "GALt]s",  "GGUp}{",  "GONR}{",  "G?Bax{",  "G?AZ^k",  "GCSzzw",  "GAkzn[",
    "GEL\\Z[", "GBd\\Z[", "GANe|w",  "GES|^[",  "GBM^]{",  "GDXcy{",  "GCXtuw",  "GIG\\[w", "GQGX}W",  "G`GZ[w",
    "GHFKvK",  "G_[sj[",  "G_[sn[",  "GcCzR{",  "GKKmi{",  "GALt^s",  "GqAix{",  "GEHkzs",  "GAjP~s",  "G@W}]k",
    "GCLZ\\[", "GAY\\Zk", "G@qi~k",  "G_hX~{",  "GAMnnw",  "G_Mz~k",  "G@Nv]{",  "G_lzrk",  "G@zRzw",  "G_lv~w",
    "GDhzu{",  "GDhzv{",  "GEnbzw",  "GtlrY{",  "G}nax{",  "G`iayw",  "G@hYtK",  "GtP@Ww",  "GsXX~{",  "GqhX~{",
    "Gdlr^{",  "G`o~f{",  "GA_jlw",  "G`Mi~_",  "G@oZN_",  "GSP@x{",  "G}hX~{",  "GqN~v{",  "G`~~~{",  "G{S~~w",
    "G_?ztw",  "G?qipk",  "G?uvZw",  "GR~v~{",  "G?CqS[",  "G?DH\\k", "G?GSY[",  "G?GTa[",  "G?Gsy{",  "G?Mq~s",
    "G?O_k[",  "G?O_ls",  "G?O_~k",  "G?Og~{",  "G?QXrK",  "G?hZtg",  "G@?]RW",  "G@@GtC",  "G@@GtK",  "G@AAWw",
    "G@GR[w",  "G@HSYs",  "G@hZtk",  "GAXttw",  "GB_g~K",  "G_ClQk",  "G`_YPK",  "G`iZY{",  "G_urPk",  "GG?O\\o",
    "G?H?|_",  "GR~~vk",  "G?GXeC",  "G?GTaW",  "GN~~~{",  "G?G^eW",  "G?G^e[",  "G?G]^w",  "G?G]^{",  "GQ`@Gs",
    "G`?CzW",  "G??CzW",  "Grqz~{",  "GqG?g[",  "G``?pK",  "GHDk}{",  "GDHG}{",  "GQh\\z{", "GTlrY{",  "G@hV~{",
    "G@qz}{",  "GDlr]{",  "GKXu|{",  "G@KuUK",  "G???^c",  "G??OVK",  "G??O^C",  "G?GU?[",  "G?CBKW",  "G?G\\z{",
    "G?rHx{",  "G@dcz[",  "G_G\\zw", "G`iiqk",  "G`qihs",  "G`r@xw",  "G@IQV{",  "GQG\\Yw", "GGEJd{",  "GPOY\\k",
    "GCQrP{",  "G?hSz[",  "G@QP}[",  "GCO\\j[", "GCOp]{",  "G?U`m{",  "GAhP[{",  "G}_xq[",  "G}h_w{",  "G?Fn~{",
    "G?NV~{",  "G?dn~{",  "GAI^~{",  "G??F~w",  "G?CV^W",  "G?AZvo",  "G?r@xw",  "GAHc{w",  "G?B\\ro", "G@hQtK",
    "G?D@\\g", "GdW}z{",  "G?CrUW",  "GhqXz{",  "G??O^O",  "G??PUW",  "G?ClQk",  "G?G\\Qk", "G_Jqps",  "G}lz~{",
    "G`N~~{",  "GwCW~{",  "G@?MFw",  "G?SsZ[",  "G@CJK{",  "G?_OzK",  "G?D@K{",  "G??Jc[",  "G@B?Ws",  "G\\YY}{",
    "G`rHx{",  "G?B~vo",  "G?Mi~{",  "GS`zro",  "GqOxs{",  "GGlVK{",  "G``HW{"};

std::vector<std::string> ListNumVertices_8 = {
    "G@Kxx{",  "G@Kx~{",  "G?_gqK",  "G?????",  "Gs\\v~w", "GF~~~{",  "G~?GW[",  "GwC^?{",  "G???GK",  "G}hPW{",
    "GqN~~{",  "GJ\\zz{", "GJ\\z~{", "G~z\\z{", "G^~~~{",  "G~~~~{",  "GI?HOg",  "G?DPV?",  "G?DPTC",  "G?`HW{",
    "G@GQSG",  "G??GW[",  "G??MPg",  "G????C",  "G???Ns",  "G?CWw{",  "G?CW~{",  "G??GOK",  "GoCQPG",  "G?CHM_",
    "G??gv_",  "G?@HcK",  "G???^w",  "GsP@xw",  "G?@Ht_",  "Gs`zro",  "G?~~~{",  "G`K?GK",  "G`?GZc",  "G??CZc",
    "G`_gqK",  "GsOiho",  "G???F{",  "G??F~{",  "G?B~~{",  "G`?G?C",  "G?O_c[",  "G?GQSK",  "G?zTjo",  "Gs@ipo",
    "G`dnfw",  "GQo~nw",  "G?z\\z{", "GwCOOK",  "GoCOZ?",  "G?xPcK",  "G@?G?C",  "G@hRSk",  "GoSv~w",  "GIiZ~w",
    "G@lv]{",  "GsOax{",  "GANLfK",  "G{O_w{",  "GANcvK",  "GCS~VK",  "G_h\\rk", "GoDPV{",  "GqGTY{",  "G`deX{",
    "Gj?HWw",  "G?DP^{",  "G?`@pk",  "G?XX~{",  "G?S~^{",  "G?qz~{",  "GJ?G^{",  "GGD_~{",  "GGC^F{",  "GGDc~{",
    "GGCZ^{",  "GGC^^{",  "GASp^{",  "GGDXv{",  "G@O}v{",  "GHCZ^{",  "GGD\\v{", "GAHX~{",  "GHC^^{",  "G@P\\~{",
    "G@pHn{",  "G`BHv{",  "G_C~V{",  "GKCZ^{",  "GGEZ^{",  "G`BH~{",  "G`C^^{",  "G@bJ~{",  "GAJ\\v{", "GGEzv{",
    "G_Ezv{",  "G@`~v{",  "GAJ\\~{", "GGDzv{",  "GGD~v{",  "GGF~v{",  "GGSs~{",  "GIGX~{",  "GIG\\~{", "GPLQ^{",
    "GJOg~{",  "GIHX~{",  "GGS~f{",  "G@XX~{",  "GAXt~{",  "G@X\\~{", "G@fbv{",  "GiGX~{",  "GIIX~{",  "GII[~{",
    "GiG\\~{", "GD`j~{",  "G@jR~{",  "GXCY^{",  "GHFH~{",  "G@S~^{",  "GCS~F{",  "GANc~{",  "GCS~^{",  "G@U~V{",
    "GAwzn{",  "GCL~V{",  "GIJ\\v{", "GAlnn{",  "GIJ\\~{", "GDSz^{",  "GCUzv{",  "G@]^n{",  "G@Z\\~{", "GALzv{",
    "GAL~v{",  "GAN~v{",  "GDYR^{",  "GhCZ^{",  "G@^Dn{",  "GhC^^{",  "GBaj~{",  "GCUrV{",  "G@UvV{",  "GAMjn{",
    "GANT^{",  "GHFK~{",  "GPOy~{",  "GDPk~{",  "GCSz^{",  "G@oy~{",  "G@hu~{",  "GB_~^{",  "GCLm~{",  "GGeZf{",
    "G_K~f{",  "G`FH~{",  "G_ox~{",  "G_K~n{",  "G@Nev{",  "G@NU^{",  "G@NJn{",  "G@Ne~{",  "GCdj~{",  "G@rvv{",
    "G_Nrv{",  "GC]r^{",  "G_Mzv{",  "G_[~n{",  "G_Nr~{",  "GgEzv{",  "G@pzv{",  "G@lu^{",  "G@p~v{",  "G@r~v{",
    "Gw?W~{",  "G@ou^{",  "GEGi~{",  "GEGm~{",  "G@Nmv{",  "G@Nm~{",  "GpHY~{",  "G@vfn{",  "G_lr~{",  "GFY^^{",
    "GCx~n{",  "GC|rn{",  "GC^r~{",  "GDxZn{",  "G@^u~{",  "G_}rn{",  "GC~r~{",  "GiIX~{",  "GDdj^{",  "GHpX~{",
    "GDLm^{",  "GJQk~{",  "GIdl~{",  "GBiZ~{",  "GAlv^{",  "GA\\t~{", "G@lu~{",  "GMhX~{",  "GRW}~{",  "GBZ\\~{",
    "GRWy~{",  "GJXX~{",  "GIL~v{",  "GIN~v{",  "GINT^{",  "G`XX~{",  "GBpl~{",  "G`X\\~{", "GqSp^{",  "GoS~f{",
    "GqHX~{",  "GhFH~{",  "GaMjn{",  "GIMk~{",  "G`Vd~{",  "GQlu^{",  "GdSz^{",  "GKszn{",  "GbM^^{",  "GQqz~{",
    "GDlr^{",  "GP\\^n{", "Golq~{",  "GElr^{",  "GEhzv{",  "GDZ^v{",  "GDZ~v{",  "GRX\\~{", "Gq\\t~{", "GS\\~f{",
    "GIm~n{",  "GI~t~{",  "GJZ\\~{", "GdX\\~{", "GD^vV{",  "GD^v^{",  "G}ox~{",  "G]qz~{",  "GF~v^{",  "GTPH~{",
    "G`S~^{",  "GwDXv{",  "G`NJn{",  "GQMi~{",  "GKdzv{",  "Gqox~{",  "GqLzv{",  "G??ZCS",  "G?hTrk",  "G`?KzW",
    "GKS~\\{", "GGF~vo",  "G@hZCc",  "G?@Hdo",  "G?XPc[",  "G???^g",  "GAI?g[",  "G?@PSo",  "G?Oc_w",  "G_D`Gs",
    "G@`AXk",  "GHdu\\{", "GAh`ks",  "GCSXJC",  "G?qaho",  "G@?La[",  "GqKsY[",  "G???~G",  "G?GQKo",  "G@??OK",
    "G??_uK",  "GwC^~{",  "G~rHx{",  "GoSv~{",  "G?mzz{",  "G~`HW{",  "GKdax{",  "G`?N~w",  "G`?Lz{",  "GICX|[",
    "G?qix{",  "G??x}{",  "G`?MXw",  "G@`@W{",  "G??Dzw",  "G??Cz[",  "GEChX[",  "GIi^bw",  "GSXTz{",  "G@G\\a[",
    "GtPHOk",  "Gv?IXW",  "Gvzax{",  "G?r@pg",  "GSP@xw",  "G?DPS{",  "GAG]|{",  "GBOk~W",  "G`G\\zw", "G_ltjs",
    "G@lv]w",  "G@zTz{",  "G_]~bk",  "G@nRnS",  "GIM}v[",  "G`iZQk",  "GSXP~w",  "GQMZ^k",  "GsXTzw",  "GINT^o",
    "GBZT[{",  "GINe|{",  "GsXPW{",  "GqYPW{",  "Gr_YX[",  "GKoXzg",  "GDxRK{",  "GSXZ`{",  "G]Og|{",  "G@luvG",
    "GSXX~c",  "GSLi~k",  "GdZDzw",  "Guhax{",  "GgN\\rk", "G}hHg{",  "GrddY{",  "GqMZj[",  "GRqZ^k",  "Gqlsz[",
    "Gsxqx{",  "GMnax{",  "GP^uu{",  "GD^vU{",  "GEl~~w",  "GrotY{",  "G`zTzw",  "GQ}rm[",  "GqluX{",  "Grqix{",
    "GRl~Ms",  "G]`@W{",  "G?urf[",  "G?\\utk", "G@dTJS",  "G@MQ^?",  "GGFLbo",  "GoDPZo",  "GoDP^o",  "GqGR[w",
    "Gr`?x[",  "GqMAXk",  "G@h^vg",  "G@X^dw",  "GCS~~w",  "GANvS{",  "GqMAh[",  "G_}ahk",  "G@h^e[",  "GBYk}{",
    "GPpX~k",  "GWS}|{",  "G@zZrk",  "G@lu~w",  "GEhXrK",  "GEhax{",  "GqGZ[{",  "GpDHzw",  "GgMP}w",  "G`hP~w",
    "GRQH}w",  "GDZDzw",  "G`ddzw",  "GoSr~w",  "GDZDz{",  "G`ddz{",  "GDZcy{",  "GqopW{",  "GoTXpk",  "GEhrS{",
    "GMhP[{",  "GqSpZ{",  "G`h\\rg", "GK^@|k",  "G`Urt[",  "GqHX~o",  "GD^Dj[",  "GDZUX{",  "G`X\\tk", "GBhmk{",
    "GdZDz{",  "GpFjq{",  "GQqzvk",  "GJqix{",  "GJM]^[",  "GqJ\\z{", "GcL~v[",  "GDZ^vw",  "GMMZ\\[", "GJMk}[",
    "GqNax{",  "GDYz~o",  "G?L?nC",  "Gq?@Ww",  "G@?]VK",  "G@GSY[",  "GOBAx{",  "G@GR[{",  "G@?Z]{",  "G@CNM[",
    "G?EJZw",  "G?G^]w",  "G?EVZ{",  "GoCAhW",  "GoCAh[",  "GA?X^O",  "G@HCyw",  "G@BDY{",  "G@?kyw",  "G?Ss^{",
    "GA?}TS",  "G@Aiu[",  "G?Eja{",  "G@@ku{",  "G?ErU{",  "G@Aiv{",  "G?C}^k",  "G?Hszs",  "G?FRX{",  "G?Er]{",
    "G_?|Qs",  "G_?ytK",  "G?O|fo",  "GGAXu[",  "G_Aix{",  "G?HXvk",  "G?K]Nc",  "GCCZ^K",  "GC_Zj[",  "G@G^Uk",
    "GGKQK[",  "GGDPS{",  "GAOpS{",  "GI?gt{",  "GI?gv{",  "GAHco{",  "G@P_|s",  "GGHSsw",  "GGO\\dw", "G@IQ]{",
    "G@DJ\\w", "G@Ok}w",  "G@EJZw",  "G@DdY{",  "G?XTtg",  "G?XTtk",  "G?Sr^w",  "G?XP~k",  "G??guK",  "G@EIzw",
    "G?UP~K",  "GCDH~[",  "G?Ss~[",  "GAG[~{",  "GAHe|{",  "G@Om|w",  "GAG]|w",  "G?Sv^w",  "G@G]UK",  "G?KuMs",
    "G@G[yw",  "GAKs][",  "G@IYu{",  "GAKuZ[",  "GAKu\\[", "GCKr]W",  "GADl\\s", "G@G}m[",  "Go@Xz{",  "G@K]m[",
    "G?S~Bc",  "GANDG{",  "GDHG~K",  "GCDhvK",  "G?czb[",  "G?L\\f[", "G?Yq|s",  "G?cznK",  "G?V`~s",  "G?@Htk",
    "G?hZlk",  "G?h\\rk", "G?ozn{",  "G?LuZs",  "G?Mi~k",  "GAEj|w",  "G?Mz~k",  "G?Mzv_",  "G?AYpo",  "G_GSZ_",
    "G@`?xw",  "GCO_~[",  "GCO_~{",  "GCOaxw",  "GQ?g}K",  "G_l@Gk",  "G`GSY[",  "G??q\\s", "G?CTZW",  "GGCmc[",
    "GCDH^K",  "GCDH^[",  "G`GR[{",  "GG_q|{",  "Go?Ax{",  "G??i|s",  "G@dTZ[",  "GGgq{{",  "GAcr\\{", "G@UJh{",
    "GCOzs{",  "G@Qi|s",  "GAJax{",  "G@P\\^k", "GAIZ~w",  "GAMPY[",  "GAcpZ[",  "GAHXvK",  "GAHkzs",  "GOPXvk",
    "G?BDz{",  "G@O|}w",  "GQAzu[",  "GAJ\\r{", "G_hP_[",  "GCUb?{",  "Gp@Gzo",  "GoDPZ{",  "GaIHg{",  "GGUTX{",
    "G?Baps",  "G??X~G",  "G@hSz[",  "GqGUXw",  "GGMP}w",  "G@hP~w",  "GaClz{",  "GBIM~w",  "G?AZRk",  "G?AZVk",
    "GALel[",  "G@TtU[",  "GIEht{",  "GAYXzk",  "G@Xs}{",  "GG]Pm[",  "G@h[z[",  "GILe|{",  "GGS}|{",  "GALt]s",
    "GGUp}{",  "GONR}{",  "G?Bax{",  "G?AZ^k",  "GCSzzw",  "GAkzn[",  "GEL\\Z[", "GBd\\Z[", "GANe|w",  "GES|^[",
    "GBM^]{",  "GDXcy{",  "GCXtuw",  "GIG\\[w", "GQGX}W",  "G`GZ[w",  "GHFKvK",  "G_[sj[",  "G_[sn[",  "GcCzR{",
    "GKKmi{",  "GALt^s",  "GqAix{",  "GEHkzs",  "GAjP~s",  "G@W}]k",  "GCLZ\\[", "GAY\\Zk", "G@qi~k",  "G_hX~{",
    "GAMnnw",  "G_Mz~k",  "G@Nv]{",  "G_lzrk",  "G@zRzw",  "G_lv~w",  "GDhzu{",  "GDhzv{",  "GEnbzw",  "GtlrY{",
    "G}nax{",  "G`iayw",  "G@hYtK",  "GtP@Ww",  "GsXX~{",  "GqhX~{",  "Gdlr^{",  "G`o~f{",  "GA_jlw",  "G`Mi~_",
    "G@oZN_",  "GSP@x{",  "G}hX~{",  "GqN~v{",  "G`~~~{",  "G{S~~w",  "G_?ztw",  "G?qipk",  "G?uvZw",  "GR~v~{",
    "G?CqS[",  "G?DH\\k", "G?GSY[",  "G?GTa[",  "G?Gsy{",  "G?Mq~s",  "G?O_k[",  "G?O_ls",  "G?O_~k",  "G?Og~{",
    "G?QXrK",  "G?hZtg",  "G@?]RW",  "G@@GtC",  "G@@GtK",  "G@AAWw",  "G@GR[w",  "G@HSYs",  "G@hZtk",  "GAXttw",
    "GB_g~K",  "G_ClQk",  "G`_YPK",  "G`iZY{",  "G_urPk",  "G???Zc",  "GG?O\\o", "G?H?|_",  "GR~~vk",  "G?GXeC",
    "G?GTaW",  "GN~~~{",  "G?G^eW",  "G?G^e[",  "G?G]^w",  "G?G]^{",  "GQ`@Gs",  "G`?CzW",  "G??CzW",  "Grqz~{",
    "GqG?g[",  "G``?pK",  "GHDk}{",  "GDHG}{",  "GQh\\z{", "GTlrY{",  "G@hV~{",  "G@qz}{",  "GDlr]{",  "GKXu|{",
    "G@KuUK",  "G???^c",  "G??OVK",  "G??O^C",  "G?GU?[",  "G?CBKW",  "G?G\\z{", "G?rHx{",  "G@dcz[",  "G_G\\zw",
    "G`iiqk",  "G`qihs",  "G`r@xw",  "G@IQV{",  "GQG\\Yw", "GGEJd{",  "GPOY\\k", "GCQrP{",  "G?hSz[",  "G@QP}[",
    "GCO\\j[", "GCOp]{",  "G?U`m{",  "GAhP[{",  "G}_xq[",  "G}h_w{",  "G?Fn~{",  "G?NV~{",  "G?dn~{",  "GAI^~{",
    "G??F~w",  "G?CV^W",  "G?AZvo",  "G?r@xw",  "GAHc{w",  "G?B\\ro", "G@hQtK",  "G?D@\\g", "GdW}z{",  "G?CrUW",
    "GhqXz{",  "G??O^O",  "G??PUW",  "G?ClQk",  "G?G\\Qk", "G_Jqps",  "G}lz~{",  "G`N~~{",  "GwCW~{",  "G@?MFw",
    "G?SsZ[",  "G@CJK{",  "G?_OzK",  "G?D@K{",  "G??Jc[",  "G@B?Ws",  "G\\YY}{", "G`rHx{",  "G?B~vo",  "G?Mi~{",
    "GS`zro",  "GqOxs{",  "GGlVK{",  "G``HW{",  "G?G\\Qg", "G?ClQg"};

std::vector<std::string> ListNumVertices_9 = {
    "H??????",  "HqGOOKF",  "HI?GPGQ",  "H?CQPJ_",  "H??[Aci",  "H???F~~",  "H????CB",  "H?~~~~~",   "HwCW?CB",
    "H`B@_WJ",  "H???ONs",  "H?????@",  "H??F~~~",  "HGC?G?@",  "H??__RF",  "H{_yqgj",  "H?aFbx{",   "H???GKF",
    "H@Kxx{~",  "H@Kxx~~",  "H????F|",  "H}q|r|}",  "H???GN~",  "HF~~vnn",  "HF~~~~~",  "H~?GW[N",   "HwCW~?^",
    "HwC^?~{",  "HqN~~~~",  "HJ\\zz|~", "HJ\\zz~~", "H~z\\z|~", "H~z\\z~~", "H^~~~~~",  "H~~~~~~",   "H?B~vrw",
    "H?B~~~~",  "H`?G?CB",  "H@GOQIA",  "H?DCP_d",  "H???GGB",  "H@G?GgI",  "H??IPio",  "HoCOPHA",   "H?G?iIW",
    "H?A?ISq",  "H@GSY^o",  "H?C[Y[n",  "H????N}",  "H??GW[N",  "H??GW^~",  "H????^p",  "H?CWw{^",   "H?CWw~~",
    "H???C\\q", "H????B~",  "HEutZhj",  "HCHCp\\V", "HqH}dS^",  "HoCOOL_",  "HwCO?SE",  "H{dQXgj",   "HoDPV~}",
    "HIISZ~}",  "H?MzvNV",  "H?S}tnn",  "H?`jm}~",  "H?h[bFr",  "H@IQ]re",  "H`?guNw",  "H`AkrT[",   "H_GouNy",
    "H@`M@~u",  "HR?MC|m",  "HR?M@~M",  "HR`HGvN",  "HCSp]Nw",  "H@iRI~w",  "HPSRK^[",  "HANLbNw",   "H?ManrM",
    "H@hZSno",  "H{`Ypgj",  "HQ?g}Rp",  "HQ?{QVr",  "H``@_^{",  "HR?G]Nw",  "HPp?g~e",  "H`Y?g~e",   "H_?|Qrr",
    "H[CGYNp",  "H`GO]Nw",  "H`IQO~e",  "HqGOW~e",  "HDYAG~e",  "Ha?{PVr",  "Ho@PW~w",  "Hq?HWzr",   "H`?L?lJ",
    "HCd@Igi",  "H?z\\z~~", "H_K~]~~",  "HCZ\\z~~", "H@zTz~~",  "H@l~]~~",  "HIiZ|~~",  "HBqj|~~",   "HHr\\z~~",
    "HDYz}~~",  "Higx}~~",  "HFXm|~~",  "HiK|[~~",  "HsXXz~~",  "HiiXz~~",  "HsSz~^~",  "HDxuz~~",   "HFZm|~~",
    "HBu~Z~~",  "HEl~~z~",  "H~?GW^~",  "HwC^?~~",  "HwCW~N~",  "HwCW~^~",  "H`rHpn~",  "HKeZb^~",   "HwC[zz~",
    "H`ouX~~",  "H`K~E~~",  "HeGh}~~",  "HTPLzz~",  "H`NDzz~",  "HwCZ~z~",  "HTPLz~~",  "HwC^~z~",   "HcKz]~~",
    "HHe^Z~~",  "HTlai^~",  "HRhTY~~",  "HwDX~r~",  "H`NUX~~",  "HwDX~v~",  "HTPi|~~",  "H}CXZ^~",   "H`^@|n~",
    "HtPHx~~",  "H`NH~f~",  "H`o~fz~",  "H{CZZ~~",  "HwD\\r~~", "H`K~Mr~",  "H`K~Mv~",  "H`rHx~~",   "H`K~M~~",
    "HKeZZ~~",  "H`NNjz~",  "HtPLzz~",  "HwC}~z~",  "H`Nez~~",  "HtPLz~~",  "HQh\\zz~", "H`Mzu^~",   "HwFXzv~",
    "H`px~v~",  "HQh\\z~~", "H`o~~z~",  "H`K~]z~",  "H`K~]~~",  "HpNay~~",  "H{dax~~",  "HjbH|~~",   "H`qzr~~",
    "H`zPz~~",  "H`qzvn~",  "HKd~vz~",  "HKd~r~~",  "HKfzrv~",  "Hfphx~~",  "HbK~]~~",  "H`}rm^~",   "HKd~v^~",
    "H`Nmz~~",  "H`N~u~~",  "HxFHy~~",  "HRiiy~~",  "Hhox}~~",  "HRhY|~~",  "H`tp~^~",  "HdKz]^~",   "H`Mzu~~",
    "H~`HW~~",  "HrhTY~~",  "HtPix~~",  "HhNUX~~",  "H{DX~^~",  "HJqkz~~",  "H`uvZz~",  "HKuzrn~",   "Hwuqx~~",
    "HqltY~~",  "HKnZvn~",  "HQlu~^~",  "HKttz~~",  "Hqh\\z~~", "HTlrY~~",  "HTXZ~z~",  "HTX\\z~~",  "HTX^~z~",
    "HqK|Y~~",  "HqKy~^~",  "H}hPW~~",  "H}GZ[~~",  "HrotY~~",  "HqNax~~",  "HtOy~^~",  "Hqgyz~~",   "HRqi~n~",
    "HhqXz~~",  "HqzPx~~",  "HuSxz^~",  "HkurX~~",  "HYU\\~z~", "HqK~]~~",  "Hktpx~~",  "Hdthzn~",   "H`mzrn~",
    "HpLy~v~",  "H`zTzz~",  "HKur~z~",  "HqL\\~^~", "HdW}z~~",  "H`lv]~~",  "H`lv~z~",  "H\\TZ\\~~", "H}Ky~^~",
    "HR]~Mv~",  "HJnL~n~",  "HhN~u~~",  "HjK~]~~",  "HRVlz~~",  "H[Uzu~~",  "HLrZt~~",  "H\\diz~~",  "HKl~e~~",
    "HRlu~Z~",  "HKnr~v~",  "HRlu~^~",  "HQl~~z~",  "H{dXz^~",  "HrrHx~~",  "HYU}t~~",  "HKu~b~~",   "H`l~vj~",
    "H`l~vn~",  "HbNl}~~",  "H`z\\z~~", "H~rHx~~",  "H^rLz~~",  "H`~~vn~",  "Hrqix~~",  "HYd}t~~",   "HtXXz~~",
    "HMutZ~~",  "HvSzZ^~",  "HvSz^^~",  "HqL~r~~",  "HRl~Mv~",  "HLnJ~n~",  "HdYz~r~",  "Hbiz~v~",   "HdYz}~~",
    "HqN~r~~",  "HtlrY~~",  "H{Sz~z~",  "HRvdz~~",  "HtX\\z~~", "H{S~~z~",  "H}hXz~~",  "H}nax~~",   "Hrvdz~~",
    "Hp^uz~~",  "HRl}~^~",  "Hq~tz~~",  "HjNm|~~",  "HJz\\z~~", "HMn~r~~",  "Hru~Z~~",  "HR~~vn~",   "H???XjA",
    "H?qahro",  "H??@GrD",  "H???]Os",  "H``?@cM",  "H?D_Sfa",  "HCD@z^~",  "H`?Ha]{",  "H`?N~z{",   "HgGqsy]",
    "HqOpS|}",  "HR?M[xk",  "HWLKm_^",  "H?LE`YT",  "H@CI?M`",  "H`?H_ZB",  "H??E@KZ",  "H@`?OKr",   "H?AA`O[",
    "H{db?{]",  "HoDczp{",  "Hqopc|m",  "Hqcxq\\f", "Hr?GOKF",  "HQQ?@KY",  "H?C@IQD",  "H??HIaW",   "H????^q",
    "HAA?PKy",  "H@BQPS^",  "HPVBG}z",  "HELZTMN",  "HDpch\\Z", "H??@GrB",  "H?CAHM^",  "HINT^~~",   "H@lv]~~",
    "H{~va{~",  "HINLn~~",  "HJaJY{~",  "H@IYvN~",  "H?B~~rx",  "H{dahWZ",  "H@Kp]jJ",  "H`?HW~r",   "H@Kp]fL",
    "HoCOZB~",  "H?BMXwz",  "H?CWvNf",  "H@GZQmN",  "H@GZIuN",  "H??Kz^{",  "H??E\\lz", "H?CWtHb",   "HoStIv{",
    "HhEJG~w",  "H?hSzZr",  "HEhcqkn",  "H}nna{~",  "HQYPOnN",  "H??P?VD",  "HoDPX`H",  "H{Sz]}~",   "HoDPXPP",
    "H~?G[|m",  "H{dbG{^",  "Hd^@|ln",  "H{S}tln",  "H}lah[^",  "H`zPpnN",  "HdYr]o~",  "HLnBj]^",   "H[UY|ln",
    "HRiiqnN",  "Htlai[~",  "HwDX}yz",  "H`o}Pnr",  "H`opc\\N", "HThQQK~",  "H_hP_^o",  "HC`PX`H",   "HC[QHNb",
    "HiGgomN",  "HwCx}zY",  "HxGW}nN",  "HwDX}yy",  "H`o~f~~",  "Hs`yppb",  "H}l?XKV",  "H_?{aOb",   "H`?M@cN",
    "H?So]Mj",  "H?KLIns",  "H?qipno",  "H?qa`_N",  "HAGOSLe",  "H?COZIa",  "H{dX`TF",  "H?qipn~",   "H?CGHFO",
    "H?O_cC[",  "H?o@Gho",  "H??OPNW",  "H??M?kw",  "H`lu^~~",  "H??WvJa",  "HwCWqGb",  "H???F~}",   "HoSsaKj",
    "H?@HeYr",  "HHqGhdN",  "HpO]@cN",  "HoCQPJ~",  "HCO__SF",  "Hr?GW^~",  "HWCW~~~",  "H@?GjEL",   "H@?OZEL",
    "H_Bbowx",  "H{dbGwZ",  "H{`rOwZ",  "H}hPOkN",  "H|pGpKV",  "H|ogqKV",  "H}gWqKf",  "H{`Xqgj",   "H{`XrOV",
    "H~zUX{~",  "HqGO?KJ",  "HWEA?WR",  "HK`@?WR",  "HqG??kM",  "H``??se",  "HQ`?@KY",  "Ho?OQoe",   "H?ChUjQ",
    "H?CKZhn",  "H?AHQkz",  "HXUQ|zM",  "H??F~z{",  "HqN~~zz",  "H`deX~{",  "HoDXvnm",  "H}hX~~~",   "H????Nx",
    "H??OONg",  "H??OPFK",  "H??@OjK",  "H??OZAH",  "H???jQK",  "H?C?JEK",  "H??_aYI",  "H????Ny",   "H???ONp",
    "H???Gva",  "H??C?\\p", "H???_Zb",  "H???OJr",  "H?`@?_N",  "H??ORAF",  "H??OON`",  "H???Oje",   "H???gZ`",
    "H???_^c",  "H???Gvc",  "H??G@fE",  "H?AAGSp",  "H?C@M?L",  "H??_aQM",  "H?GQCCL",  "H??_eOM",   "H`?G?F~",
    "H??E\\hw", "HQG[IVr",  "HCDlAVr",  "H`GIkze",  "HBIEB]]",  "HQD_[Vp",  "Hs`rY{~",  "HXNUS|^",   "HxU_w~N",
    "HoFvQ{~",  "HqGTY~~",  "HRAZU^~",  "HWNVS|^",  "HoDPV~~",  "HsOax~~",  "HqJTY{~",  "HyGW}mn",   "H{O_w~~",
    "HqddY{~",  "Hk`rX}^",  "H`GO\\|}", "H@?tY~N"};

std::vector<std::string> ListNumVertices_10 = {
    "I??????~w",  "I????????",  "I??F~~~~w",   "I`?G?C??G",  "I?@@?_G`w",   "I??OS?dOo",  "I??OPHAe?",   "I????AF]G",
    "I?AAHGICW",  "I@Kxx{~Nw",  "I@Kxx{~~w",   "Is\\v~~}~W", "IF~~~~~~w",   "I~{?GKF@w",  "I~?GW^oBw",   "Iv~vVL^Nw",
    "IJ\\zz|~^w", "IJ\\zz|~~w", "I~~v]}~^w",   "I^~~~~~~w",  "I~~~~~~~w",   "I?????@?W",  "I???GKF@w",   "I????B~~w",
    "IsaBzx{^?",  "I?B~~~~~w",  "I`?GW?@?W",   "I@`??CBNG",  "II?GOGQGO",   "I?COPHAw?",  "I?CGI_agG",   "I???G[ws?",
    "I???????G",  "I?AAHKVBw",  "I@GOOH@__",   "I?????@~g",  "I?????@~w",   "I?CGGC`s?",  "I??GHCWsG",   "I????CA?W",
    "I??GIOqk?",  "I????CB?w",  "I????CB~w",   "I??E@GW?w",  "I???F~~~w",   "I??GW[NBw",  "I??GW[N~w",   "I?CWw{^Fw",
    "I?CWw{^~w",  "I????FaFO",  "I?????B~o",   "IoCOOGaOO",  "I?~~vrv|o",   "I?~~~~~~w",  "IwCW?CB?w",   "I`?G]?oBw",
    "IsP@OkWHG",  "I?hPcterW",  "I?C]dPcww",   "I_O_hWZLw",  "I??EdPKL?",   "IoCOW@@GO",  "IwCO?CI@O",   "IoCOOL?GG",
    "IwCO?CBB?",  "IA?HcPfnW",  "IsOExw{XW",   "IoCQPJ~~o",  "IIGSSL~~o",   "I@GUUM~^o",  "I?EQzU|xw",   "I?BLYwz|w",
    "I?BT^?|Uo",  "I`?DIu[ZG",  "I?`H]es]?",   "IR?HWzKoo",  "I`?NE_lF_",   "IW@[pqTQw",  "I`i\\QleUW",  "ICQjlt{}?",
    "I}iAxxLLg",  "Ihe@WxVwo",  "IRCm]YiTW",   "I@qixrTwo",  "IqHHcpNLo",   "IqIAXl\\N_", "ICqhqlx\\_",  "IAJLn@ZNO",
    "IsLGjERZG",  "IRrJC_fEw",  "ItP@yglaw",   "I``Cz]{^?",  "IGEBGz`eg",   "I`?NE_nFo",  "I?[_k^atG",   "I@GZSncuG",
    "I@GZSncu?",  "IWDApy[`w",  "IoD@Is{f_",   "IRGOW^Kow",  "I?EZZjQiW",   "I?LtdXZr_",  "I?^@lhRig",   "IgNSQcnLo",
    "I`hPWxbcw",  "Iw?yspTIw",  "Ija?X_J@w",   "I{?BWoTBW",  "IRaIACbFG",   "IqS`K?X@w",  "It?GY@FMO",   "I`B@_WJ}?",
    "ItPG?CfEo",  "IFAIPWjp_",  "Iq??XyiTg",   "I@?uQQL{O",  "IGAZOqPwW",   "IJaD_XDAw",  "IF?G]YiX_",   "ItPG?CrBo",
    "Is?JY_pBW",  "I?X_sespo",  "Ii??Xyidg",   "Iq`@OgfEw",  "Is?IXXQKW",   "IqMA`OF@w",  "Ija?pGJ@w",   "I?emBXQKg",
    "I__HGlo}G",  "IoCOQKf}?",  "IWAQWoXwW",   "IsP@OgfEw",  "IQ`F`GXBW",   "IsOPAWVIw",  "IIGOSMF}?",   "IqGODTUBw",
    "I``@xaHPW",  "I@`MEWyL_",  "IukAHLUJO",   "I~~DI[vFw",  "I~qkzXZLw",   "I???h?HpG",  "I????SE{G",   "I???BGY`_",
    "It?IQGiDO",  "IG_UXzs}O",  "I_A_tt}^_",   "I?O__E@BG",  "IJPHPKYDW",   "I?@?RaQJG",  "IQQ@HKYEW",   "Ir?GWWJ@w",
    "I`?M@_K?w",  "Iv?GOKFY?",  "I???pHG`G",   "I?C@?N?BG",  "IugOXhJX_",   "I?_aCwuU_",  "I?cqcGbQW",   "I@Q@QK^kw",
    "ICHCPLRFw",  "IOD?g\\bew", "I@`CJcnFw",   "I??GjQoRg",  "IoCOPHA~w",   "I????Wqoo",  "I???OGoow",   "I{~~fd^Nw",
    "ICS}||~~w",  "I?v\\||~^w", "I`?N~~~~w",   "I~ze}w~Nw",  "I~~DJK^Fw",   "Itlai[~Nw",  "IQGm_|~~w",   "IwCO]}~^w",
    "I~zf?{^Fw",  "I?rMX{~Nw",  "I~fLQ{}Xw",   "I?qkz\\^Nw", "IvzfA{}Fw",   "I?C^~~{~G",  "I?@Yx{~zw",   "Is`zro~Nw",
    "I??D||~^w",  "I??F~~}~W",  "I~?GW]^Zo",   "I~}AHKVBw",  "ITlAIK~No",   "I?qi`e^Zw",  "I`?M\\\\]NW", "IITX`Sffw",
    "I@KpW~Tqw",  "IoCWzdkfw",  "I?eRRI^Zw",   "I`?M]W~No",  "I@KpW~RrW",   "I?qahq^Zw",  "I`?L[|mVW",   "IS`rQo~No",
    "I`?L[|lVg",  "IoSs[\\r[w", "I`?G?F~~w",   "I???w}vZw",  "I??GW]~^o",   "IIP@xw{fw",  "I`?HpnKrG",   "IwCWONbFo",
    "I`qa`_NBw",  "I_GTddMRW",  "I_CfClMMW",   "IC`PhPHFw",  "ICdAHgiFw",   "I_COXofxg",  "I_COXgjv_",   "I??BIu{nG",
    "I_?@Gs~~?",  "I_COXclv_",  "I??GW^x|_",   "I??E\\X]NO", "I``H?cNBw",   "I?CWo]dWw",  "I`iRA_NBw",   "I`?LSleUW",
    "IC`PX`HFw",  "Is_BjX[N?",  "I??E\\X\\N_", "I_GTd`NRo",  "I?XPc]M[W",   "I???E}}^_",  "I???CtVJw",   "I????B~~o",
    "IsPH@CYDO",  "Is?AXW[[?",  "I??EdPMLO",   "I??@pjKr?",  "I??FF?]FO",   "IGD@G_H`w",  "I??G[``@w",   "I_?__QF]G",
    "I?`@?_NBo",  "I??CPKUQW",  "IskAJLUN?",   "I{cBIkkFG",  "IIISYgj}?",   "IHQKjOV}?",  "I}mBIkmFW",   "I@hisny|W",
    "I??OQ?``g",  "Is?HGtcU?",  "Is?@WxcU?",   "I@QLQlSiG",  "I~rH`cNBw",   "Is`b?{]]?",  "Is?WaSiX?",   "Is?WpDHX?",
    "Iv{?GLFHo",  "I}z\\bcnFw", "I}nnnp^Nw",   "I?deLStUg",  "ISP@OmeUW",   "Is@?xOXX?",  "It?IQKWHG",   "IkGHWn\\{o",
    "Ir?MHqnVo",  "Io?Y`QF]?",  "I`?N~bLNW",   "IwDX}yy~w",  "I{dbG{^~w",   "I~?G[|m~w",  "I}lah[^~w",   "IwC^?~{~w",
    "I`o~f~~~w",  "I?GTbHWb?",  "IQiiqk~~w",   "It{?ILFL_",  "I_?BSgcEG",   "I}hX~~~~w",  "I?AKagjX_",   "IrrH`enVo",
    "I??G`TEdW",  "I??GaGaeG",  "I??Gagjxg",   "I??GhtFig",  "I??GiK|yg",   "I??GiyiHw",  "I??Gqk|jw",   "I??GsTVLo",
    "I??Gs_fVG",  "I??HGlZno",  "I??HGlZnw",   "I??HGlZzO",  "I??HOhB`w",   "I??IHKVew",  "I??I_ofxW",   "I??KHORPW",
    "I??KJOVMg",  "I??KOkiPW",  "I??KOtVLw",   "I??KQKSIG",  "I??OPHA}W",   "I??OPHB`w",  "I??OPPE`o",   "I??OQ?``w",
    "I??OQKeeW",  "I??OQKfeW",  "I??OQKfmw",   "I??OQKxxg",  "I??OS?dOw",   "I??OS@XHw",  "I??OSLFHw",   "I??OS_bRG",
    "I??OXLZjW",  "I??OXLZzO",  "I??OY?`ew",   "I??OY?`}W",  "I??OYKhcg",   "I??OYKzjw",  "I??OYOflo",   "I??O[K|Uo",
    "I??O[K|Uw",  "I??Op\\efW", "I??Oq[mfW",   "I??Oqwm~g",  "I??Os\\NJw",  "I??P?XAeW",  "I??P?XIco",   "I??P?\\Xf_",
    "I??POpFxo",  "I??PYyz~O",  "I??P\\t^~g",  "I??Psxm~?",  "I??Q?slfg",   "I??QXYRzG",  "I??Rswn~G",   "I??T@GNUW",
    "I??WQ?b`w",  "I??XPqdSw",  "I??YcOfJo",   "I??[QOfLo",  "I??_Pe^Vo",   "I??_Pe^Vw",  "I??_QCRfW",   "I??_Xu^Vo",
    "I??_Xu^Vw",  "I??_YOVkw",  "I??_Yczno",   "I??_[pFMg",  "I??__PB`o",   "I??__QFPo",  "I??__QFPw",   "I??__QwPw",
    "I??__TE`W",  "I??__TFmW",  "I??__\\YfO",  "I??__\\YfW", "I??_`pnfo",   "I??_`pnfw",  "I??_aOMbo",   "I??_g]~^o",
    "I??_g]~^w",  "I??aGWRmw",  "I??aWwrmW",   "I??aWwrmw",  "I??a_[znO",   "I??a`O\\fo", "I??c?cJPW",   "I??gS`BDg",
    "I??gaCJcW",  "I??ggxJgw",  "I??gkTJLW",   "I??g||~~_",  "I??wt\\yTw",  "I??yRQVmg",  "I??yTD\\mg",  "I??|AtMmW",
    "I?@?PKUeW",  "I?@?XGReW",  "I?@?_Ob`w",   "I?@?_ObfG",  "I?@?_SffW",   "I?@?_Wb}w",  "I?@?gWb`w",   "I?@?gWb~w",
    "I?@@?_Hfg",  "I?@@?_JbO",  "I?@@?_JnW",   "I?@@?cJ`w",  "I?@@?cLeg",   "I?@@?gJeg",  "I?@@?kNfg",   "I?@@?kNnw",
    "I?@@G_H_w",  "I?@@G_H`w",  "I?@@GkNnw",   "I?@GPCRbW",  "I?@G_Sfko",   "I?@G_Sfmw",  "I?@G_Sfnw",   "I?@G`CJdW",
    "I?@X?UBMW",  "I?@YTs}hw",  "I?A?_TFHw",   "I?A?_pNJo",  "I?A?_pNJw",   "I?A?gXBGw",  "I?A@G\\ZNW",  "I?AAown^W",
    "I?C?HdLjw",  "I?C?IWrjg",  "I?C?Igjx_",   "I?C?XHRxO",  "I?C?Y?dcw",   "I?C?iGjbw",  "I?C?i[mbw",   "I?C?i[nlw",
    "I?C?i[nzg",  "I?C?jGmbo",  "I?C@?\\Tjg",  "I?C@AKMkG",  "I?C@AKMkW",   "I?C@AKMnW",  "I?C@GXRzw",   "I?C@i^knO",
    "I?C@{XtzG",  "I?CA?KbbG",  "I?CAHCLnG",   "I?CAO[vxO",  "I?CAXWVlw",   "I?CAXWVzG",  "I?CCZk}zW",   "I?CGJCueo",
    "I?CIZYv^g",  "I?CKQgf\\g", "I?COJCmew",   "I?COPlMbw",  "I?COPlMz_",   "I?COPlN~w",  "I?COQ[ubg",   "I?COQ[vzg",
    "I?COQknzg",  "I?COS?vRo",  "I?COS?vRw",   "I?COSlNJw",  "I?COVKuRg",   "I?CQH\\mmo", "I?CS@LNZo",   "I?C_AcNj_",
    "I?C_GTPw_",  "I?C_GdHbw",  "I?C_GdJbO",   "I?C_i\\mmo", "I?C_i\\m|o",  "I?C_kHJYo",  "I?CaIgjfw",   "I?CcH`JRo",
    "I?CgaKj{w",  "I?D??cjh_",  "I?DB@aLbo",   "I?DOX]r]w",  "I?DPXxZfw",   "I?DPXxZ~_",  "I?D__Kjao",   "I?E@`xm^_",
    "I?EA?kmJo",  "I?EGGlJIw",  "I?EqYonkw",   "I?G?WdDlw",  "I?G?_\\Uho",  "I?G@Ig]eo",  "I?GAGcLlW",   "I?GG_LPxw",
    "I?GG_NPXg",  "I?GGa_Fzg",  "I?GGznuno",   "I?GO?\\I`g", "I?GOY?rbW",   "I?GPb]]~_",  "I?GW^dylg",   "I?GWaCeaW",
    "I?GX?uFPw",  "I?G_k`JRo",  "I?G_qK\\v_",  "I?Gpqx]vo",  "I?H?OiBDg",   "I?HBg~x~O",  "I?HOGSrew",   "I?HOGSrfw",
    "I?I@ph\\Vo", "I?IZryv}W",  "I?KKAKV\\g",  "I?KQCKM[W",  "I?LF?kN[W",   "I?OH?_F`o",  "I?OKbMuNo",   "I?OPYK|fw",
    "I?OS?oeHo",  "I?OXpxVng",  "I?O_gxbfg",   "I?O_yK|kw",  "I?Oa_}{n_",   "I?Oaju}no",  "I?Ogz~}~o",   "I?OwpufTw",
    "I?Pb`}]}W",  "I?SDHlsbW",  "I?SGh\\vnw",  "I?SOX]u\\o", "I?SjSnt\\o",  "I?StACLkW",  "I?WIG}v\\o",  "I?_?GctQw",
    "I?_?iwn^g",  "I?_@GhBNG",  "I?_@G|]\\o",  "I?_GHCRPW",  "I?_OHtM^_",   "I?_QXYRZG",  "I?`@?[]Fo",   "I?cOoXFHw",
    "I?cPHXRRg",  "I?cPIStRg",  "I?cWaKeAw",   "I?oPGpBHW",  "I@??LXZr_",   "I@??OXFf_",  "I@??OXFfg",   "I@??OlMhw",
    "I@??i[mfG",  "I@??i[mfW",  "I@?G?TFdo",   "I@?G?TFdw",  "I@?GhTFfW",   "I@?IIgjfg",  "I@?IOmFYW",   "I@?IpM\\vO",
    "I@?OH]IQg",  "I@?OOXBbg",  "I@?SQOFJG",   "I@?YtKnVW",  "I@@?pMKUW",   "I@@GHOZco",  "I@@IHUZfo",   "I@@XXpZhw",
    "I@@g{pjdw",  "I@C?ICLwg",  "I@COYUtZg",   "I@CdQm]ZW",  "I@DGtLjlg",   "I@GTQl]jW",  "I@GY}xn~g",   "I@HH?]VUw",
    "I@ODG|[hW",  "I@OH?lEeW",  "I@O[P\\VXw",  "I@O_{zMzo",  "I@OhozFTw",   "I@Ohun[Xw",  "I@P@gyNjo",   "I@QWqcnXw",
    "I@Sg\\CVTw", "I@_OXLZVo",  "I@g?gxfVg",   "I@h?GkYWw",  "I@o\\`\\VxW", "IA??GWjko",  "IA?G?Kjlw",   "IA?HHhJmg",
    "IA?_xU\\vO", "IAAKPln]o",  "IAC_[GbQW",   "IAC_g\\jnw", "IACppXNzw",   "IAG@I]]fo",  "IAGGaKefw",   "IAKPY^vzo",
    "IAl`g|N{w",  "IC?`O|]Vo",  "IC?`Px]Vo",   "IC@@oK\\YW", "IC@xo|zjw",   "ICCAHGJzG",  "ICCGQCvXo",   "ICGOIUcQg",
    "ICN@G\\rqw", "IC\\P[^vzo", "ID?GGpFIw",   "ID@@xW\\Qw", "IDG@I\\]Fo",  "IG??_SFnw",  "IG?G_gJhg",   "IG?Gigjf_",
    "IG?Gigj}W",  "IG?QX}}no",  "IG?XPYVUw",   "IG?oYMZMo",  "IG@@gyJfG",   "IGC@?gNbo",  "IGCOQmmjo",   "IGCP?\\EeW",
    "IGCaKORBW",  "IGCcYWVKw",  "IGCgQmYHw",   "IGCiYkvmw",  "IGDKQkmKw",   "IGDPQ[umW",  "IGDQXkn~w",   "IGEY`TNLw",
    "IG_GGsqPW",  "IHCHhXVqw",  "IHEGiKjqw",   "IHIRYw^uw",  "IKGOOLH`g",   "IKGQWyfUw",  "IKGxctNrg",   "IOH?IeYFo",
    "IOOWQKzLo",  "IOOoo\\F[w", "IOQ@I_ZFo",   "IP?Oq[{Rg",  "IQCpQ[Vbg",   "IQGYKOjDw",  "IQXto}\\rw",  "I_Cj?mJ[W",
    "I_Cj?mJ\\W", "I_GZ?uF\\W", "I_Hpc\\^]o",  "I_L?Xc\\`w", "I_O_`_Nbo",   "I_P`c[]MW",  "I`?OYClew",   "I`o|KtVYw",
    "IqMu[|vZw",  "I???CtUJ_",  "I?C?GWqoo",   "I???gWww?",  "I}q@xw{Pw",   "I?COeYiX_",  "I`__a?H@g",   "I?qkqsuXW",
    "I??E@KZ~?",  "I??E@KZ~w",  "I_?__QF]?",   "IcG@IGHCg",  "IukH@LFT_",   "I}iAzg\\Jg", "I??E@GW~?",   "I?hOGQRWo",
    "I~?GW]pRg",  "I_hPOkrrw",  "ICd@IgiFw",   "ITiZbTURW",  "It?IQOeDO",   "It?GYDKKO",  "Is@@WoXX?",   "It?IQScDG",
    "Is?GYWq[?",  "Is?HOlK[?",  "Is?OyOhX?",   "It?IIOiDO",  "It?GW\\_SG",  "Is?GW\\o[?", "I?GQSHzlw",   "I?D@K`nfw",
    "ISPDthnVo",  "I?_Y@E^Zw",  "I??HOj?{?",   "IQQ???fEo",  "I@`CA?XH_",   "IqGOO?B@W",  "IqGO?CDAg",   "IoCQ@?EAW",
    "IK_PA?H@g",  "IqGO??FAo",  "IWEA??LCo",   "IK`@??LCo",  "I_GSA?MKO",   "Iq?HWzz|o",  "I`_giVBN?",   "I???F~}~_",
    "I??F~z{~?",  "I|v^BKzFw",  "I`?KQKSIG",   "I_Kpc\\M{?", "I@_XIpbBw",   "ICOH@`E`o",  "I`r@pi^Zo",   "I`IQUK~~_",
    "IQhPW~euW",  "IEhcqkn~?",  "I_hPpnlv_",   "I`BHva^Zo",  "IQ??GVILW",   "I??GhUiTW",  "I~zLa[vFw",   "IK?CQKSIG",
    "IS`AA?~No",  "Io?WrAF]G",  "Is@ipr~~o",   "IoDPV~~~w",  "IwCx}zY~w",   "IyGW}mn~w",  "IXNUS|^~w",   "IqddY{~~w",
    "I{O_w~~~w",  "IoFvQ{~~w",  "IWNVS|^~w",   "IxGW}nN~w",  "I{N?w|n~w",   "IqJTY{~~w",  "IANLfN~~w",   "IsOax~~~w",
    "Ik`rX}^~w",  "Is`rY{~~w",  "IqGTY~~~w"};

}; // namespace

namespace
{

#define BIAS6 63
#define MAXBYTE 126
#define SMALLN 62
#define SMALLISHN 258047
#define TOPBIT6 32
#define C6MASK 63

#define SIZELEN(n) ((n) <= SMALLN ? 1 : ((n) <= SMALLISHN ? 4 : 8))
/* length of size code in bytes */
#define G6BODYLEN(n) (((size_t)(n) / 12) * ((size_t)(n) - 1) + (((size_t)(n) % 12) * ((size_t)(n) - 1) + 11) / 12)
#define G6LEN(n) (SIZELEN(n) + G6BODYLEN(n))
/* exact graph6 string length excluding \n\0
   This twisted expression works up to n=227023 in 32-bit arithmetic
   and for larger n if size_t has 64 bits.  */
#define D6BODYLEN(n) ((n) * (size_t)((n) / 6) + (((n) * (size_t)((n) % 6) + 5) / 6))
#define D6LEN(n) (1 + SIZELEN(n) + D6BODYLEN(n))
/* exact digraph6 string length excluding \n\0
   This twisted expression works up to n=160529 in 32-bit arithmetic
   and for larger n if size_t has 64 bits.  */

#define B(i) (1 << ((i) - 1))
#define M(i) ((1 << (i)) - 1)

/* Remove errno: too hard to get portable without configuration
 * #if defined(__unix) || defined(__unix__) || defined(unix) || \
    defined(__ppc__)
#include <errno.h>
#else
int errno = 0;
#endif
#define ABORT(msg) {if (errno != 0) perror(msg); exit(1);}
*/

/* extern long ftell(FILE*);   Should be in stdio.h  */

#define SWBOOLEAN(c, boool)                                                                                            \
    if (sw == c)                                                                                                       \
        boool = TRUE;
#define SWINT(c, boool, val, id)                                                                                       \
    if (sw == c)                                                                                                       \
    {                                                                                                                  \
        boool = TRUE;                                                                                                  \
        arg_int(&arg, &val, id);                                                                                       \
    }
#define SWRANGE(c, boool, val1, val2, id)                                                                              \
    if (sw == c)                                                                                                       \
    {                                                                                                                  \
        boool = TRUE;                                                                                                  \
        arg_range(&arg, &val1, &val2, id);                                                                             \
    }

typedef unsigned long setword;

typedef setword set;
typedef setword graph;
typedef int boolean;

static setword bit[32] = {020000000000, 010000000000, 04000000000, 02000000000, 01000000000, 0400000000, 0200000000,
                          0100000000,   040000000,    020000000,   010000000,   04000000,    02000000,   01000000,
                          0400000,      0200000,      0100000,     040000,      020000,      010000,     04000,
                          02000,        01000,        0400,        0200,        0100,        040,        020,
                          010,          04,           02,          01};
static int leftbit[] = {8, 7, 6, 6, 5, 5, 5, 5, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
static int labelorg = 0;

#define WORDSIZE 32
#define FIRSTBIT(x)                                                                                                    \
    ((x) & 037777600000 ? ((x) & 037700000000 ? leftbit[((x) >> 24) & 0377] : 8 + leftbit[(x) >> 16])                  \
                        : ((x) & 0177400 ? 16 + leftbit[(x) >> 8] : 24 + leftbit[x]))
#define BITMASK(x)                                                                                                     \
    (017777777777 >> (x)) /* setword whose rightmost                                                                   \
WORDSIZE-x-1 (numbered) bits are 1 and the rest 0 (0 <= x < WORDSIZE) */
#define TIMESWORDSIZE(w) ((w) << 5)
#define SETWD(pos) ((pos) >> 5)
#define SETBT(pos) ((pos) & 037)
#define ISELEMENT(setadd, pos) (((setadd)[SETWD(pos)] & bit[SETBT(pos)]) != 0)
#define ADDELEMENT(setadd, pos) ((setadd)[SETWD(pos)] |= bit[SETBT(pos)])
#define GRAPHROW(g, v, m) ((set *)(g) + (long)(v) * (long)(m))

#define FALSE 0
#define TRUE 1

/************************************************************************/

#ifndef SEEK_SET
#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2
#endif

#ifndef SEEK_CUR
#define SEEK_CUR SEEK_CURRENT
#endif

static long ogf_linelen;

/************************************************************************/

static void gt_abort(const char *msg) /* Write message and halt. */
{
    throw MyException(msg);
}

/*****************************************************************************
 *                                                                            *
 *  itos(i,s) converts the int i to a nul-terminated decimal character        *
 *  string s.  The value returned is the number of characters excluding       *
 *  the nul.                                                                  *
 *                                                                            *
 *  GLOBALS ACCESSED: NONE                                                    *
 *                                                                            *
 *****************************************************************************/

static int itos(int i, char *s)
{
    int digit, j, k;
    char c;
    int ans;

    if (i < 0)
    {
        k = 0;
        i = -i;
        j = 1;
        s[0] = '-';
    }
    else
    {
        k = -1;
        j = 0;
    }

    do
    {
        digit = i % 10;
        i = i / 10;
        s[++k] = digit + '0';
    } while (i);

    s[k + 1] = '\0';
    ans = k + 1;

    for (; j < k; ++j, --k)
    {
        c = s[j];
        s[j] = s[k];
        s[k] = c;
    }

    return (ans);
}

/*****************************************************************************
 *                                                                            *
 *  nextelement(set1,m,pos) = the position of the first element in set set1   *
 *  which occupies a position greater than pos.  If no such element exists,   *
 *  the value is -1.  pos can have any value less than n, including negative  *
 *  values.                                                                   *
 *                                                                            *
 *  GLOBALS ACCESSED: none                                                    *
 *                                                                            *
 *****************************************************************************/

static int nextelement(set *set1, int m, int pos)
{
    setword setwd;
    int w;

    if (pos < 0)
    {
        w = 0;
        setwd = set1[0];
    }
    else
    {
        w = SETWD(pos);
        setwd = set1[w] & BITMASK(SETBT(pos));
    }

    for (;;)
    {
        if (setwd != 0)
            return (TIMESWORDSIZE(w) + FIRSTBIT(setwd));
        if (++w == m)
            return -1;
        setwd = set1[w];
    }
}

/*********************************************************************/

// static char*
//    showg_getline(FILE* f)     /* read a line with error checking */
//           /* includes \n (if present) and \0.
//        Immediate EOF causes NULL return. */
//{
//    DYNALLSTAT(char, s, s_sz);
//    int c;
//    long i;

//   DYNALLOC1(char, s, s_sz, 500, "showg_getline");

//   i = 0;
//   while ((c = getc(f)) != EOF && c != '\n')
//   {
//      if (i == s_sz - 2) DYNREALLOC(char, s, s_sz, s_sz + 1000, "showg_getline");
//      s[i++] = c;
//   }

//   if (i == 0 && c == EOF) return NULL;

//   if (c == '\n') s[i++] = '\n';
//   s[i] = '\0';
//   return s;
//}

/****************************************************************************/

int graphsize(const char *s)
/* Get size of graph out of graph6, digraph6 or sparse6 string. */
{
    const char *p;
    int n;

    if (s[0] == ':' || s[0] == '&')
        p = s + 1;
    else
        p = s;
    n = *p++ - BIAS6;

    if (n > SMALLN)
    {
        n = *p++ - BIAS6;
        if (n > SMALLN)
        {
            n = *p++ - BIAS6;
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
        else
        {
            n = (n << 6) | (*p++ - BIAS6);
            n = (n << 6) | (*p++ - BIAS6);
        }
    }
    return n;
}

/****************************************************************************/

static void stringtograph(const char *s, graph *g, int m)
/* Convert string (graph6, digraph6 or sparse6 format) to graph. */
/* Assumes g is big enough to hold it.   */
{
    const char *p;
    int n, i, j, k, v, x, nb, need;
    size_t ii;
    set *gi, *gj;
    boolean done;

    n = graphsize(s);
    if (n == 0)
        return;

    p = s + (s[0] == ':' || s[0] == '&') + SIZELEN(n);

    if (TIMESWORDSIZE(m) < n)
        gt_abort(">E stringtograph: impossible m value\n");

    for (ii = m * (size_t)n; --ii > 0;)
        g[ii] = 0;
    g[0] = 0;

    if (s[0] != ':' && s[0] != '&') /* graph6 format */
    {
        k = 1;
        for (j = 1; j < n; ++j)
        {
            gj = GRAPHROW(g, j, m);

            for (i = 0; i < j; ++i)
            {
                if (--k == 0)
                {
                    k = 6;
                    x = *(p++) - BIAS6;
                }

                if ((x & TOPBIT6))
                {
                    gi = GRAPHROW(g, i, m);
                    ADDELEMENT(gi, j);
                    ADDELEMENT(gj, i);
                }
                x <<= 1;
            }
        }
    }
    else if (s[0] == '&')
    {
        k = 1;
        for (i = 0; i < n; ++i)
        {
            gi = GRAPHROW(g, i, m);

            for (j = 0; j < n; ++j)
            {
                if (--k == 0)
                {
                    k = 6;
                    x = *(p++) - BIAS6;
                }

                if ((x & TOPBIT6))
                {
                    ADDELEMENT(gi, j);
                }
                x <<= 1;
            }
        }
    }
    else /* sparse6 format */
    {
        for (i = n - 1, nb = 0; i != 0; i >>= 1, ++nb)
        {
        }

        k = 0;
        v = 0;
        done = FALSE;
        while (!done)
        {
            if (k == 0)
            {
                x = *(p++);
                if (x == '\n' || x == '\0')
                {
                    done = TRUE;
                    continue;
                }
                else
                {
                    x -= BIAS6;
                    k = 6;
                }
            }
            if ((x & B(k)))
                ++v;
            --k;

            need = nb;
            j = 0;
            while (need > 0 && !done)
            {
                if (k == 0)
                {
                    x = *(p++);
                    if (x == '\n' || x == '\0')
                    {
                        done = TRUE;
                        continue;
                    }
                    else
                    {
                        x -= BIAS6;
                        k = 6;
                    }
                }
                if (need >= k)
                {
                    j = (j << k) | (x & M(k));
                    need -= k;
                    k = 0;
                }
                else
                {
                    k -= need;
                    j = (j << need) | ((x >> k) & M(need));
                    need = 0;
                }
            }
            if (done)
                continue;

            if (j > v)
                v = j;
            else if (v < n)
            {
                ADDELEMENT(GRAPHROW(g, v, m), j);
                ADDELEMENT(GRAPHROW(g, j, m), v);
            }
        }
    }
}

/************************************************************************/

#define LABELORG 0 /* number of first vertex (any integer >= 0) */
#define LINELEN 78 /* max characters per line (0 = no limit) */

static FILE *infile, *outfile;
static long nin;

/*****************************************************************************
 *                                                                            *
 *  putsetx(f,set1,curlenp,linelength,m,compress,start)   writes the set      *
 *  set1 to file f using at most linelength characters per line (excluding    *
 *  '\n').   Set elements less than or equal to start are ignored.            *
 *  *curlenp is the number of characters on the line so far; it is updated.   *
 *  A range j1,j1+1,...,j2 for j2-j1>=2 is written as "j1:j2" if compress     *
 *  is nonzero (eg. TRUE); otherwise each element is written separately.      *
 *  No final '\n' is written.  labelorg is used.                              *
 *                                                                            *
 *  FUNCTIONS CALLED: nextelement(),itos()                                    *
 *                                                                            *
 *****************************************************************************/

void myputsetx(Vertex source, std::set<SortedEdge> &edges, set *set1, int *curlenp, int linelength, int m,
               boolean compress, int start)
{
    int slen, j1, j2;
    char s[40];
    boolean first;

    first = TRUE;
    j1 = start;
    while ((j1 = nextelement(set1, m, j1)) >= 0)
    {
        j2 = j1;
        if (compress)
        {
            while (nextelement(set1, m, j2) == j2 + 1)
                ++j2;
            if (j2 == j1 + 1)
                j2 = j1;
        }
        slen = itos(j1 + labelorg, s);
        if (j2 >= j1 + 2)
        {
            s[slen] = ':';
            slen += 1 + itos(j2 + labelorg, &s[slen + 1]);
        }

        if (*curlenp + slen + 1 >= linelength && linelength > 0)
        {
            // fprintf(f, "\n ");
            *curlenp = 1;
        }
        if (first)
        {
            // fprintf(f, "%s", s);
            const Vertex target = std::stoi(s);
            edges.insert(SortedEdge{std::min(source, target), std::max(source, target)});
            *curlenp += slen;
            first = FALSE;
        }
        else
        {
            // fprintf(f, " %s", s);
            const Vertex target = std::stoi(s);
            edges.insert(SortedEdge{std::min(source, target), std::max(source, target)});
            *curlenp += slen + 1;
        }
        j1 = j2;
    }
}

/*****************************************************************************
 *                                                                            *
 *  STOLEN FROM naututil.c                                                    *
 *  putgraphx(f,g,linelength,m,n) writes a list of the edges of g to f        *
 *  using at most linelength characters per line (excluding '\n').            *
 *  If triang, only write the upper triangle.                                 *
 *  labelorg is used.                                                         *
 *                                                                            *
 *****************************************************************************/

void myputgraphx(graph *g, int m, int n, std::set<SortedEdge> &edges)
{
    constexpr int linelength = 1000;
    int i, curlen;
    set *pg;

    for (i = 0, pg = g; i < n; ++i, pg += m)
    {
        Vertex source = i + labelorg;
        // fprintf(f, "%3d : ", i + labelorg);
        curlen = 7;
        const bool triang = false;
        myputsetx(source, edges, pg, &curlen, linelength, m, FALSE, triang ? i - 1 : -1);
        // fprintf(f, ";\n");
    }
}

} // namespace

std::unique_ptr<UndirectedGraph> UndirectedGraphFromG6::Create(std::string line)
{
    std::set<SortedEdge> edges;
    const int n = graphsize(line.c_str());
    const int m = (n + WORDSIZE - 1) / WORDSIZE;
    std::vector<setword> buffer(m * n);
    graph *g = buffer.data();

    stringtograph(line.c_str(), g, m);
    myputgraphx(g, m, n, edges);

    auto result = std::make_unique<UndirectedGraph>(n, line);
    for (auto e : edges)
    {
        result->addEdge(e.first, e.second);
    }
    return result;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_3()
{
    return ListNumVertices_3;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_4()
{
    return ListNumVertices_4;
}

std::unique_ptr<Graph::GraphUsc> UndirectedGraphFromG6::CreateConnected(std::string g6)
{
    const auto undirected = Create(g6);
    return std::make_unique<Graph::GraphUsc>(*undirected);
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_5_connected()
{
    return ListNumVertices_5_connected;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_5()
{
    return ListNumVertices_5;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_6_connected()
{
    return ListNumVertices_6_connected;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_6()
{
    return ListNumVertices_6;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_7_connected()
{
    return ListNumVertices_7_connected;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_7()
{
    return ListNumVertices_7;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_8_connected()
{
    return ListNumVertices_8_connected;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_8()
{
    return ListNumVertices_8;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_9()
{
    return ListNumVertices_9;
}

const std::vector<std::string> &UndirectedGraphFromG6::getListNumVertices_10()
{
    return ListNumVertices_10;
}

std::vector<std::unique_ptr<Graph::IGraphUs>> UndirectedGraphFromG6::getDisconnectedGraphs(
    const std::vector<std::string> &list)
{

    std::vector<std::unique_ptr<Graph::IGraphUs>> result;
    for (const auto &g6 : list)
    {
        auto ugraph = UndirectedGraphFromG6::Create(g6);
        if (!ugraph->isConnected())
        {
            result.emplace_back(std::move(ugraph));
        }
    }
    return result;
}

std::vector<std::unique_ptr<Graph::IGraphUs>> UndirectedGraphFromG6::getGraphs(const std::vector<std::string> &list)
{
    std::vector<std::unique_ptr<Graph::IGraphUs>> result;
    for (const auto &g6 : list)
    {
        result.emplace_back(UndirectedGraphFromG6::Create(g6));
    }
    return result;
}
