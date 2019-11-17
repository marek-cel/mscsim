/****************************************************************************//*
 * Copyright (C) 2019 Marek M. Cel
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom
 * the Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 ******************************************************************************/

#include <nav/nav_DataBase.h>

#include <cstdio>
#include <fstream>
#include <iostream>

#include <QDomElement>
#include <QFile>

#include <Common.h>

#include <fdm/fdm_Path.h>

#include <fdm/utils/fdm_String.h>
#include <fdm/utils/fdm_Units.h>
#include <fdm/utils/fdm_WGS84.h>

////////////////////////////////////////////////////////////////////////////////

using namespace nav;

////////////////////////////////////////////////////////////////////////////////

DataBase* DataBase::_instance = NULLPTR;

////////////////////////////////////////////////////////////////////////////////

int DataBase::getFreqDME( const char *chan )
{
    if      ( 0 == fdm::String::icompare( chan, "001X" ) ) return 1025000;
    else if ( 0 == fdm::String::icompare( chan, "001Y" ) ) return 1025000;
    else if ( 0 == fdm::String::icompare( chan, "002X" ) ) return 1026000;
    else if ( 0 == fdm::String::icompare( chan, "002Y" ) ) return 1026000;
    else if ( 0 == fdm::String::icompare( chan, "003X" ) ) return 1027000;
    else if ( 0 == fdm::String::icompare( chan, "003Y" ) ) return 1027000;
    else if ( 0 == fdm::String::icompare( chan, "004X" ) ) return 1028000;
    else if ( 0 == fdm::String::icompare( chan, "004Y" ) ) return 1028000;
    else if ( 0 == fdm::String::icompare( chan, "005X" ) ) return 1029000;
    else if ( 0 == fdm::String::icompare( chan, "005Y" ) ) return 1029000;
    else if ( 0 == fdm::String::icompare( chan, "006X" ) ) return 1030000;
    else if ( 0 == fdm::String::icompare( chan, "006Y" ) ) return 1030000;
    else if ( 0 == fdm::String::icompare( chan, "007X" ) ) return 1031000;
    else if ( 0 == fdm::String::icompare( chan, "007Y" ) ) return 1031000;
    else if ( 0 == fdm::String::icompare( chan, "008X" ) ) return 1032000;
    else if ( 0 == fdm::String::icompare( chan, "008Y" ) ) return 1032000;
    else if ( 0 == fdm::String::icompare( chan, "009X" ) ) return 1033000;
    else if ( 0 == fdm::String::icompare( chan, "009Y" ) ) return 1033000;
    else if ( 0 == fdm::String::icompare( chan, "010X" ) ) return 1034000;
    else if ( 0 == fdm::String::icompare( chan, "010Y" ) ) return 1034000;
    else if ( 0 == fdm::String::icompare( chan, "011X" ) ) return 1035000;
    else if ( 0 == fdm::String::icompare( chan, "011Y" ) ) return 1035000;
    else if ( 0 == fdm::String::icompare( chan, "012X" ) ) return 1036000;
    else if ( 0 == fdm::String::icompare( chan, "012Y" ) ) return 1036000;
    else if ( 0 == fdm::String::icompare( chan, "013X" ) ) return 1037000;
    else if ( 0 == fdm::String::icompare( chan, "013Y" ) ) return 1037000;
    else if ( 0 == fdm::String::icompare( chan, "014X" ) ) return 1038000;
    else if ( 0 == fdm::String::icompare( chan, "014Y" ) ) return 1038000;
    else if ( 0 == fdm::String::icompare( chan, "015X" ) ) return 1039000;
    else if ( 0 == fdm::String::icompare( chan, "015Y" ) ) return 1039000;
    else if ( 0 == fdm::String::icompare( chan, "016X" ) ) return 1040000;
    else if ( 0 == fdm::String::icompare( chan, "016Y" ) ) return 1040000;
    else if ( 0 == fdm::String::icompare( chan, "017X" ) ) return 1041000;
    else if ( 0 == fdm::String::icompare( chan, "017Y" ) ) return 1041000;
    else if ( 0 == fdm::String::icompare( chan, "018X" ) ) return 1042000;
    else if ( 0 == fdm::String::icompare( chan, "018Y" ) ) return 1042000;
    else if ( 0 == fdm::String::icompare( chan, "019X" ) ) return 1043000;
    else if ( 0 == fdm::String::icompare( chan, "019Y" ) ) return 1043000;
    else if ( 0 == fdm::String::icompare( chan, "020X" ) ) return 1044000;
    else if ( 0 == fdm::String::icompare( chan, "020Y" ) ) return 1044000;
    else if ( 0 == fdm::String::icompare( chan, "021X" ) ) return 1045000;
    else if ( 0 == fdm::String::icompare( chan, "021Y" ) ) return 1045000;
    else if ( 0 == fdm::String::icompare( chan, "022X" ) ) return 1046000;
    else if ( 0 == fdm::String::icompare( chan, "022Y" ) ) return 1046000;
    else if ( 0 == fdm::String::icompare( chan, "023X" ) ) return 1047000;
    else if ( 0 == fdm::String::icompare( chan, "023Y" ) ) return 1047000;
    else if ( 0 == fdm::String::icompare( chan, "024X" ) ) return 1048000;
    else if ( 0 == fdm::String::icompare( chan, "024Y" ) ) return 1048000;
    else if ( 0 == fdm::String::icompare( chan, "025X" ) ) return 1049000;
    else if ( 0 == fdm::String::icompare( chan, "025Y" ) ) return 1049000;
    else if ( 0 == fdm::String::icompare( chan, "026X" ) ) return 1050000;
    else if ( 0 == fdm::String::icompare( chan, "026Y" ) ) return 1050000;
    else if ( 0 == fdm::String::icompare( chan, "027X" ) ) return 1051000;
    else if ( 0 == fdm::String::icompare( chan, "027Y" ) ) return 1051000;
    else if ( 0 == fdm::String::icompare( chan, "028X" ) ) return 1052000;
    else if ( 0 == fdm::String::icompare( chan, "028Y" ) ) return 1052000;
    else if ( 0 == fdm::String::icompare( chan, "029X" ) ) return 1053000;
    else if ( 0 == fdm::String::icompare( chan, "029Y" ) ) return 1053000;
    else if ( 0 == fdm::String::icompare( chan, "030X" ) ) return 1054000;
    else if ( 0 == fdm::String::icompare( chan, "030Y" ) ) return 1054000;
    else if ( 0 == fdm::String::icompare( chan, "031X" ) ) return 1055000;
    else if ( 0 == fdm::String::icompare( chan, "031Y" ) ) return 1055000;
    else if ( 0 == fdm::String::icompare( chan, "032X" ) ) return 1056000;
    else if ( 0 == fdm::String::icompare( chan, "032Y" ) ) return 1056000;
    else if ( 0 == fdm::String::icompare( chan, "033X" ) ) return 1057000;
    else if ( 0 == fdm::String::icompare( chan, "033Y" ) ) return 1057000;
    else if ( 0 == fdm::String::icompare( chan, "034X" ) ) return 1058000;
    else if ( 0 == fdm::String::icompare( chan, "034Y" ) ) return 1058000;
    else if ( 0 == fdm::String::icompare( chan, "035X" ) ) return 1059000;
    else if ( 0 == fdm::String::icompare( chan, "035Y" ) ) return 1059000;
    else if ( 0 == fdm::String::icompare( chan, "036X" ) ) return 1060000;
    else if ( 0 == fdm::String::icompare( chan, "036Y" ) ) return 1060000;
    else if ( 0 == fdm::String::icompare( chan, "037X" ) ) return 1061000;
    else if ( 0 == fdm::String::icompare( chan, "037Y" ) ) return 1061000;
    else if ( 0 == fdm::String::icompare( chan, "038X" ) ) return 1062000;
    else if ( 0 == fdm::String::icompare( chan, "038Y" ) ) return 1062000;
    else if ( 0 == fdm::String::icompare( chan, "039X" ) ) return 1063000;
    else if ( 0 == fdm::String::icompare( chan, "039Y" ) ) return 1063000;
    else if ( 0 == fdm::String::icompare( chan, "040X" ) ) return 1064000;
    else if ( 0 == fdm::String::icompare( chan, "040Y" ) ) return 1064000;
    else if ( 0 == fdm::String::icompare( chan, "041X" ) ) return 1065000;
    else if ( 0 == fdm::String::icompare( chan, "041Y" ) ) return 1065000;
    else if ( 0 == fdm::String::icompare( chan, "042X" ) ) return 1066000;
    else if ( 0 == fdm::String::icompare( chan, "042Y" ) ) return 1066000;
    else if ( 0 == fdm::String::icompare( chan, "043X" ) ) return 1067000;
    else if ( 0 == fdm::String::icompare( chan, "043Y" ) ) return 1067000;
    else if ( 0 == fdm::String::icompare( chan, "044X" ) ) return 1068000;
    else if ( 0 == fdm::String::icompare( chan, "044Y" ) ) return 1068000;
    else if ( 0 == fdm::String::icompare( chan, "045X" ) ) return 1069000;
    else if ( 0 == fdm::String::icompare( chan, "045Y" ) ) return 1069000;
    else if ( 0 == fdm::String::icompare( chan, "046X" ) ) return 1070000;
    else if ( 0 == fdm::String::icompare( chan, "046Y" ) ) return 1070000;
    else if ( 0 == fdm::String::icompare( chan, "047X" ) ) return 1071000;
    else if ( 0 == fdm::String::icompare( chan, "047Y" ) ) return 1071000;
    else if ( 0 == fdm::String::icompare( chan, "048X" ) ) return 1072000;
    else if ( 0 == fdm::String::icompare( chan, "048Y" ) ) return 1072000;
    else if ( 0 == fdm::String::icompare( chan, "049X" ) ) return 1073000;
    else if ( 0 == fdm::String::icompare( chan, "049Y" ) ) return 1073000;
    else if ( 0 == fdm::String::icompare( chan, "050X" ) ) return 1074000;
    else if ( 0 == fdm::String::icompare( chan, "050Y" ) ) return 1074000;
    else if ( 0 == fdm::String::icompare( chan, "051X" ) ) return 1075000;
    else if ( 0 == fdm::String::icompare( chan, "051Y" ) ) return 1075000;
    else if ( 0 == fdm::String::icompare( chan, "052X" ) ) return 1076000;
    else if ( 0 == fdm::String::icompare( chan, "052Y" ) ) return 1076000;
    else if ( 0 == fdm::String::icompare( chan, "053X" ) ) return 1077000;
    else if ( 0 == fdm::String::icompare( chan, "053Y" ) ) return 1077000;
    else if ( 0 == fdm::String::icompare( chan, "054X" ) ) return 1078000;
    else if ( 0 == fdm::String::icompare( chan, "054Y" ) ) return 1078000;
    else if ( 0 == fdm::String::icompare( chan, "055X" ) ) return 1079000;
    else if ( 0 == fdm::String::icompare( chan, "055Y" ) ) return 1079000;
    else if ( 0 == fdm::String::icompare( chan, "056X" ) ) return 1080000;
    else if ( 0 == fdm::String::icompare( chan, "056Y" ) ) return 1080000;
    else if ( 0 == fdm::String::icompare( chan, "057Y" ) ) return 1081000;
    else if ( 0 == fdm::String::icompare( chan, "058X" ) ) return 1082000;
    else if ( 0 == fdm::String::icompare( chan, "058Y" ) ) return 1082000;
    else if ( 0 == fdm::String::icompare( chan, "059X" ) ) return 1083000;
    else if ( 0 == fdm::String::icompare( chan, "059Y" ) ) return 1083000;
    else if ( 0 == fdm::String::icompare( chan, "060X" ) ) return 1084000;
    else if ( 0 == fdm::String::icompare( chan, "060Y" ) ) return 1084000;
    else if ( 0 == fdm::String::icompare( chan, "061X" ) ) return 1085000;
    else if ( 0 == fdm::String::icompare( chan, "061Y" ) ) return 1085000;
    else if ( 0 == fdm::String::icompare( chan, "062X" ) ) return 1086000;
    else if ( 0 == fdm::String::icompare( chan, "062Y" ) ) return 1086000;
    else if ( 0 == fdm::String::icompare( chan, "063X" ) ) return 1087000;
    else if ( 0 == fdm::String::icompare( chan, "063Y" ) ) return 1087000;
    else if ( 0 == fdm::String::icompare( chan, "064X" ) ) return 1088000;
    else if ( 0 == fdm::String::icompare( chan, "064Y" ) ) return 1088000;
    else if ( 0 == fdm::String::icompare( chan, "065X" ) ) return 1089000;
    else if ( 0 == fdm::String::icompare( chan, "065Y" ) ) return 1089000;
    else if ( 0 == fdm::String::icompare( chan, "066X" ) ) return 1090000;
    else if ( 0 == fdm::String::icompare( chan, "066Y" ) ) return 1090000;
    else if ( 0 == fdm::String::icompare( chan, "067X" ) ) return 1091000;
    else if ( 0 == fdm::String::icompare( chan, "067Y" ) ) return 1091000;
    else if ( 0 == fdm::String::icompare( chan, "068X" ) ) return 1092000;
    else if ( 0 == fdm::String::icompare( chan, "068Y" ) ) return 1092000;
    else if ( 0 == fdm::String::icompare( chan, "069X" ) ) return 1093000;
    else if ( 0 == fdm::String::icompare( chan, "069Y" ) ) return 1093000;
    else if ( 0 == fdm::String::icompare( chan, "070X" ) ) return 1094000;
    else if ( 0 == fdm::String::icompare( chan, "070Y" ) ) return 1094000;
    else if ( 0 == fdm::String::icompare( chan, "071X" ) ) return 1095000;
    else if ( 0 == fdm::String::icompare( chan, "071Y" ) ) return 1095000;
    else if ( 0 == fdm::String::icompare( chan, "072X" ) ) return 1096000;
    else if ( 0 == fdm::String::icompare( chan, "072Y" ) ) return 1096000;
    else if ( 0 == fdm::String::icompare( chan, "073X" ) ) return 1097000;
    else if ( 0 == fdm::String::icompare( chan, "073Y" ) ) return 1097000;
    else if ( 0 == fdm::String::icompare( chan, "074X" ) ) return 1098000;
    else if ( 0 == fdm::String::icompare( chan, "074Y" ) ) return 1098000;
    else if ( 0 == fdm::String::icompare( chan, "075X" ) ) return 1099000;
    else if ( 0 == fdm::String::icompare( chan, "075Y" ) ) return 1099000;
    else if ( 0 == fdm::String::icompare( chan, "076X" ) ) return 1100000;
    else if ( 0 == fdm::String::icompare( chan, "076Y" ) ) return 1100000;
    else if ( 0 == fdm::String::icompare( chan, "077X" ) ) return 1101000;
    else if ( 0 == fdm::String::icompare( chan, "077Y" ) ) return 1101000;
    else if ( 0 == fdm::String::icompare( chan, "078X" ) ) return 1102000;
    else if ( 0 == fdm::String::icompare( chan, "078Y" ) ) return 1102000;
    else if ( 0 == fdm::String::icompare( chan, "079X" ) ) return 1103000;
    else if ( 0 == fdm::String::icompare( chan, "079Y" ) ) return 1103000;
    else if ( 0 == fdm::String::icompare( chan, "080X" ) ) return 1104000;
    else if ( 0 == fdm::String::icompare( chan, "080Y" ) ) return 1104000;
    else if ( 0 == fdm::String::icompare( chan, "081X" ) ) return 1105000;
    else if ( 0 == fdm::String::icompare( chan, "081Y" ) ) return 1105000;
    else if ( 0 == fdm::String::icompare( chan, "082X" ) ) return 1106000;
    else if ( 0 == fdm::String::icompare( chan, "082Y" ) ) return 1106000;
    else if ( 0 == fdm::String::icompare( chan, "083X" ) ) return 1107000;
    else if ( 0 == fdm::String::icompare( chan, "083Y" ) ) return 1107000;
    else if ( 0 == fdm::String::icompare( chan, "084X" ) ) return 1108000;
    else if ( 0 == fdm::String::icompare( chan, "084Y" ) ) return 1108000;
    else if ( 0 == fdm::String::icompare( chan, "085X" ) ) return 1109000;
    else if ( 0 == fdm::String::icompare( chan, "085Y" ) ) return 1109000;
    else if ( 0 == fdm::String::icompare( chan, "086X" ) ) return 1110000;
    else if ( 0 == fdm::String::icompare( chan, "086Y" ) ) return 1110000;
    else if ( 0 == fdm::String::icompare( chan, "087X" ) ) return 1111000;
    else if ( 0 == fdm::String::icompare( chan, "087Y" ) ) return 1111000;
    else if ( 0 == fdm::String::icompare( chan, "088X" ) ) return 1112000;
    else if ( 0 == fdm::String::icompare( chan, "088Y" ) ) return 1112000;
    else if ( 0 == fdm::String::icompare( chan, "089X" ) ) return 1113000;
    else if ( 0 == fdm::String::icompare( chan, "089Y" ) ) return 1113000;
    else if ( 0 == fdm::String::icompare( chan, "090X" ) ) return 1114000;
    else if ( 0 == fdm::String::icompare( chan, "090Y" ) ) return 1114000;
    else if ( 0 == fdm::String::icompare( chan, "091X" ) ) return 1115000;
    else if ( 0 == fdm::String::icompare( chan, "091Y" ) ) return 1115000;
    else if ( 0 == fdm::String::icompare( chan, "092X" ) ) return 1116000;
    else if ( 0 == fdm::String::icompare( chan, "092Y" ) ) return 1116000;
    else if ( 0 == fdm::String::icompare( chan, "093X" ) ) return 1117000;
    else if ( 0 == fdm::String::icompare( chan, "093Y" ) ) return 1117000;
    else if ( 0 == fdm::String::icompare( chan, "094X" ) ) return 1118000;
    else if ( 0 == fdm::String::icompare( chan, "094Y" ) ) return 1118000;
    else if ( 0 == fdm::String::icompare( chan, "095X" ) ) return 1119000;
    else if ( 0 == fdm::String::icompare( chan, "095Y" ) ) return 1119000;
    else if ( 0 == fdm::String::icompare( chan, "096X" ) ) return 1120000;
    else if ( 0 == fdm::String::icompare( chan, "096Y" ) ) return 1120000;
    else if ( 0 == fdm::String::icompare( chan, "097X" ) ) return 1121000;
    else if ( 0 == fdm::String::icompare( chan, "097Y" ) ) return 1121000;
    else if ( 0 == fdm::String::icompare( chan, "098X" ) ) return 1122000;
    else if ( 0 == fdm::String::icompare( chan, "098Y" ) ) return 1122000;
    else if ( 0 == fdm::String::icompare( chan, "099X" ) ) return 1123000;
    else if ( 0 == fdm::String::icompare( chan, "099Y" ) ) return 1123000;
    else if ( 0 == fdm::String::icompare( chan, "100X" ) ) return 1124000;
    else if ( 0 == fdm::String::icompare( chan, "100Y" ) ) return 1124000;
    else if ( 0 == fdm::String::icompare( chan, "101X" ) ) return 1125000;
    else if ( 0 == fdm::String::icompare( chan, "101Y" ) ) return 1125000;
    else if ( 0 == fdm::String::icompare( chan, "102X" ) ) return 1126000;
    else if ( 0 == fdm::String::icompare( chan, "102Y" ) ) return 1126000;
    else if ( 0 == fdm::String::icompare( chan, "103X" ) ) return 1127000;
    else if ( 0 == fdm::String::icompare( chan, "103Y" ) ) return 1127000;
    else if ( 0 == fdm::String::icompare( chan, "104X" ) ) return 1128000;
    else if ( 0 == fdm::String::icompare( chan, "104Y" ) ) return 1128000;
    else if ( 0 == fdm::String::icompare( chan, "105X" ) ) return 1129000;
    else if ( 0 == fdm::String::icompare( chan, "105Y" ) ) return 1129000;
    else if ( 0 == fdm::String::icompare( chan, "106X" ) ) return 1130000;
    else if ( 0 == fdm::String::icompare( chan, "106Y" ) ) return 1130000;
    else if ( 0 == fdm::String::icompare( chan, "107X" ) ) return 1131000;
    else if ( 0 == fdm::String::icompare( chan, "107Y" ) ) return 1131000;
    else if ( 0 == fdm::String::icompare( chan, "108X" ) ) return 1132000;
    else if ( 0 == fdm::String::icompare( chan, "108Y" ) ) return 1132000;
    else if ( 0 == fdm::String::icompare( chan, "109X" ) ) return 1133000;
    else if ( 0 == fdm::String::icompare( chan, "109Y" ) ) return 1133000;
    else if ( 0 == fdm::String::icompare( chan, "110X" ) ) return 1134000;
    else if ( 0 == fdm::String::icompare( chan, "110Y" ) ) return 1134000;
    else if ( 0 == fdm::String::icompare( chan, "111X" ) ) return 1135000;
    else if ( 0 == fdm::String::icompare( chan, "111Y" ) ) return 1135000;
    else if ( 0 == fdm::String::icompare( chan, "112X" ) ) return 1136000;
    else if ( 0 == fdm::String::icompare( chan, "112Y" ) ) return 1136000;
    else if ( 0 == fdm::String::icompare( chan, "113X" ) ) return 1137000;
    else if ( 0 == fdm::String::icompare( chan, "113Y" ) ) return 1137000;
    else if ( 0 == fdm::String::icompare( chan, "114X" ) ) return 1138000;
    else if ( 0 == fdm::String::icompare( chan, "114Y" ) ) return 1138000;
    else if ( 0 == fdm::String::icompare( chan, "115X" ) ) return 1139000;
    else if ( 0 == fdm::String::icompare( chan, "115Y" ) ) return 1139000;
    else if ( 0 == fdm::String::icompare( chan, "116X" ) ) return 1140000;
    else if ( 0 == fdm::String::icompare( chan, "116Y" ) ) return 1140000;
    else if ( 0 == fdm::String::icompare( chan, "117X" ) ) return 1141000;
    else if ( 0 == fdm::String::icompare( chan, "117Y" ) ) return 1141000;
    else if ( 0 == fdm::String::icompare( chan, "118X" ) ) return 1142000;
    else if ( 0 == fdm::String::icompare( chan, "118Y" ) ) return 1142000;
    else if ( 0 == fdm::String::icompare( chan, "119X" ) ) return 1143000;
    else if ( 0 == fdm::String::icompare( chan, "119Y" ) ) return 1143000;
    else if ( 0 == fdm::String::icompare( chan, "120X" ) ) return 1144000;
    else if ( 0 == fdm::String::icompare( chan, "120Y" ) ) return 1144000;
    else if ( 0 == fdm::String::icompare( chan, "121X" ) ) return 1145000;
    else if ( 0 == fdm::String::icompare( chan, "121Y" ) ) return 1145000;
    else if ( 0 == fdm::String::icompare( chan, "122X" ) ) return 1146000;
    else if ( 0 == fdm::String::icompare( chan, "122Y" ) ) return 1146000;
    else if ( 0 == fdm::String::icompare( chan, "123X" ) ) return 1147000;
    else if ( 0 == fdm::String::icompare( chan, "123Y" ) ) return 1147000;
    else if ( 0 == fdm::String::icompare( chan, "124X" ) ) return 1148000;
    else if ( 0 == fdm::String::icompare( chan, "124Y" ) ) return 1148000;
    else if ( 0 == fdm::String::icompare( chan, "125X" ) ) return 1149000;
    else if ( 0 == fdm::String::icompare( chan, "125Y" ) ) return 1149000;
    else if ( 0 == fdm::String::icompare( chan, "126X" ) ) return 1150000;
    else if ( 0 == fdm::String::icompare( chan, "126Y" ) ) return 1150000;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int DataBase::getFreqDME( int freq_ils )
{
    if      ( freq_ils == 108100 ) return 1042000;
    else if ( freq_ils == 108150 ) return 1042000;
    else if ( freq_ils == 108300 ) return 1044000;
    else if ( freq_ils == 108350 ) return 1044000;
    else if ( freq_ils == 108500 ) return 1046000;
    else if ( freq_ils == 108550 ) return 1046000;
    else if ( freq_ils == 108700 ) return 1048000;
    else if ( freq_ils == 108750 ) return 1048000;
    else if ( freq_ils == 108900 ) return 1050000;
    else if ( freq_ils == 108950 ) return 1050000;
    else if ( freq_ils == 109100 ) return 1052000;
    else if ( freq_ils == 109150 ) return 1052000;
    else if ( freq_ils == 109300 ) return 1054000;
    else if ( freq_ils == 109350 ) return 1054000;
    else if ( freq_ils == 109500 ) return 1056000;
    else if ( freq_ils == 109550 ) return 1056000;
    else if ( freq_ils == 109700 ) return 1058000;
    else if ( freq_ils == 109750 ) return 1058000;
    else if ( freq_ils == 109900 ) return 1060000;
    else if ( freq_ils == 109950 ) return 1060000;
    else if ( freq_ils == 110100 ) return 1062000;
    else if ( freq_ils == 110150 ) return 1062000;
    else if ( freq_ils == 110300 ) return 1064000;
    else if ( freq_ils == 110350 ) return 1064000;
    else if ( freq_ils == 110500 ) return 1066000;
    else if ( freq_ils == 110550 ) return 1066000;
    else if ( freq_ils == 110700 ) return 1068000;
    else if ( freq_ils == 110750 ) return 1068000;
    else if ( freq_ils == 110900 ) return 1070000;
    else if ( freq_ils == 110950 ) return 1070000;
    else if ( freq_ils == 111100 ) return 1072000;
    else if ( freq_ils == 111150 ) return 1072000;
    else if ( freq_ils == 111300 ) return 1074000;
    else if ( freq_ils == 111350 ) return 1074000;
    else if ( freq_ils == 111500 ) return 1076000;
    else if ( freq_ils == 111550 ) return 1076000;
    else if ( freq_ils == 111700 ) return 1078000;
    else if ( freq_ils == 111750 ) return 1078000;
    else if ( freq_ils == 111900 ) return 1080000;
    else if ( freq_ils == 111950 ) return 1080000;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

int DataBase::getFreqGS( int freq_ils )
{
    if      ( freq_ils == 108100 ) return 334700;
    else if ( freq_ils == 108150 ) return 334550;
    else if ( freq_ils == 108300 ) return 334100;
    else if ( freq_ils == 108350 ) return 333950;
    else if ( freq_ils == 108500 ) return 329900;
    else if ( freq_ils == 108550 ) return 329750;
    else if ( freq_ils == 108700 ) return 330500;
    else if ( freq_ils == 108750 ) return 330350;
    else if ( freq_ils == 108900 ) return 329300;
    else if ( freq_ils == 108950 ) return 329150;
    else if ( freq_ils == 109100 ) return 331400;
    else if ( freq_ils == 109150 ) return 331250;
    else if ( freq_ils == 109300 ) return 332000;
    else if ( freq_ils == 109350 ) return 331850;
    else if ( freq_ils == 109500 ) return 332600;
    else if ( freq_ils == 109550 ) return 332450;
    else if ( freq_ils == 109700 ) return 333200;
    else if ( freq_ils == 109750 ) return 333050;
    else if ( freq_ils == 109900 ) return 333800;
    else if ( freq_ils == 109950 ) return 333650;
    else if ( freq_ils == 110100 ) return 334400;
    else if ( freq_ils == 110150 ) return 334250;
    else if ( freq_ils == 110300 ) return 335000;
    else if ( freq_ils == 110350 ) return 334850;
    else if ( freq_ils == 110500 ) return 329600;
    else if ( freq_ils == 110550 ) return 329450;
    else if ( freq_ils == 110700 ) return 330200;
    else if ( freq_ils == 110750 ) return 330050;
    else if ( freq_ils == 110900 ) return 330800;
    else if ( freq_ils == 110950 ) return 330650;
    else if ( freq_ils == 111100 ) return 331700;
    else if ( freq_ils == 111150 ) return 331550;
    else if ( freq_ils == 111300 ) return 332300;
    else if ( freq_ils == 111350 ) return 332150;
    else if ( freq_ils == 111500 ) return 332900;
    else if ( freq_ils == 111550 ) return 332750;
    else if ( freq_ils == 111700 ) return 333500;
    else if ( freq_ils == 111750 ) return 333350;
    else if ( freq_ils == 111900 ) return 331100;
    else if ( freq_ils == 111950 ) return 330950;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////

DataBase::DataBase()
{
    QFile file( fdm::Path::get( "data/nav/nav_db.xml" ).c_str() );

    if ( file.open(QFile::ReadOnly | QFile::Text) )
    {
        QDomDocument doc;

        doc.setContent( &file, false );

        QDomElement rootNode = doc.documentElement();

        if ( rootNode.tagName() == "nav_db" )
        {
            QDomElement nodeAPT = rootNode.firstChildElement( "apt" );
            QDomElement nodeILS = rootNode.firstChildElement( "ils" );
            QDomElement nodeNAV = rootNode.firstChildElement( "nav" );
            QDomElement nodeRWY = rootNode.firstChildElement( "rwy" );

            if ( !nodeAPT.isNull() )
            {
                QDomElement fileNode = nodeAPT.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseAPT( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeILS.isNull() )
            {
                QDomElement fileNode = nodeILS.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseILS( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeNAV.isNull() )
            {
                QDomElement fileNode = nodeNAV.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseNAV( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }

            if ( !nodeRWY.isNull() )
            {
                QDomElement fileNode = nodeRWY.firstChildElement( "file" );

                while ( !fileNode.isNull() )
                {
                    parseRWY( fileNode.text().toStdString().c_str() );
                    fileNode = fileNode.nextSiblingElement( "file" );
                }
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

DataBase::~DataBase()
{

}

////////////////////////////////////////////////////////////////////////////////

void DataBase::init()
{
    std::cout << "DataBase::init()" << std::endl;
}

////////////////////////////////////////////////////////////////////////////////

double DataBase::getTrueBearing( const std::string &str_bear, const std::string &str_magvar )
{
    double bear = fdm::Units::deg2rad( fdm::String::toDouble( str_bear ) );

    bool true_bear = toupper( str_bear.c_str()[ str_bear.length() - 1 ] ) == 'T';

    std::vector< std::string > temp = fdm::String::split( str_magvar, " " );

    if ( temp.size() > 0 && !true_bear )
    {
        if ( temp.at( 0 ).length() == 7 )
        {
            double coef = toupper( temp.at( 0 ).c_str()[ 0 ] ) == 'E' ? 1.0 : -1.0;

            std::string str_deg;
            std::string str_dec;

            str_deg += temp.at( 0 ).c_str()[ 1 ];
            str_deg += temp.at( 0 ).c_str()[ 2 ];
            str_deg += temp.at( 0 ).c_str()[ 3 ];

            str_dec += temp.at( 0 ).c_str()[ 4 ];
            str_dec += temp.at( 0 ).c_str()[ 5 ];
            str_dec += temp.at( 0 ).c_str()[ 6 ];

            double magvar = fdm::String::toDouble( str_deg )
                          + fdm::String::toDouble( str_dec ) / 1000.0;

            bear += coef * fdm::Units::deg2rad( magvar );
        }
    }

    return bear;
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseAPT( const char *path )
{
    std::fstream file( fdm::Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 23 )
                {
                    APT apt;

                    apt.ident = cols.at( 0 );
                    apt.name  = cols.at( 1 );
                    apt.icao  = cols.at( 3 );

                    apt.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at(  8 ) ) );
                    apt.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 10 ) ) );

                    _list_apt.push_back( apt );
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseILS( const char *path )
{
    std::fstream file( fdm::Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 29 )
                {
                    if ( cols.at( 2 ).length() > 0 )
                    {
                        ILS ils;

                        switch( toupper( cols.at( 2 ).c_str()[ 0 ] ) )
                        {
                            default:  ils.type = ILS::NONE; break;
                            case 'D': ils.type = ILS::DME;  break;
                            case 'Z': ils.type = ILS::LOC;  break;
                            case 'G': ils.type = ILS::GS;   break;
                            case 'B': ils.type = ILS::BCM;  break;
                            case 'I': ils.type = ILS::IM;   break;
                            case 'M': ils.type = ILS::MM;   break;
                            case 'O': ils.type = ILS::OM;   break;
                        }

                        if ( ils.type != ILS::NONE )
                        {
                            ils.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 15 ) ) );
                            ils.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 17 ) ) );

                            ils.elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 10 ) ) );

                            ils.true_bearing = getTrueBearing( cols.at( 24 ), cols.at( 22 ) );
                            ils.glide_slope  = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 7 ) ) );

                            ils.freq = fdm::String::toDouble( cols.at( 5 ) );

                            if ( ils.type == ILS::DME && cols.at( 6 ).length() > 0 )
                            {
                                ils.freq = getFreqDME( cols.at( 6 ).c_str() );
                            }

                            fdm::WGS84::Geo position_geo;

                            position_geo.lat = ils.lat;
                            position_geo.lon = ils.lon;
                            position_geo.alt = ils.elev;

                            ils.pos_wgs = fdm::WGS84::geo2wgs( position_geo );

                            _list_ils.push_back( ils );
                        }
                    }
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseNAV( const char *path )
{
    std::fstream file( fdm::Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 32 )
                {
                    NAV nav;

                    switch( fdm::String::toInt( cols.at( 1 ) ) )
                    {
                        default:           nav.type = NAV::NONE;    break;
                        case NAV::VOR:     nav.type = NAV::VOR;     break;
                        case NAV::VORTAC:  nav.type = NAV::VORTAC;  break;
                        case NAV::TACAN:   nav.type = NAV::TACAN;   break;
                        case NAV::VOR_DME: nav.type = NAV::VOR_DME; break;
                        case NAV::NDB:     nav.type = NAV::NDB;     break;
                        case NAV::NDB_DME: nav.type = NAV::NDB_DME; break;
                        case NAV::LOCATOR: nav.type = NAV::LOCATOR; break;
                        case NAV::DME:     nav.type = NAV::DME;     break;
                    }

                    if ( nav.type != NAV::NONE )
                    {
                        nav.lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 18 ) ) );
                        nav.lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 20 ) ) );

                        nav.elev = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 23 ) ) );

                        nav.freq = fdm::String::toDouble( cols.at( 8 ) );

                        fdm::WGS84::Geo position_geo;

                        position_geo.lat = nav.lat;
                        position_geo.lon = nav.lon;
                        position_geo.alt = nav.elev;

                        nav.pos_wgs = fdm::WGS84::geo2wgs( position_geo );

                        _list_nav.push_back( nav );
                    }
                }
            }

            i_line++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////

void DataBase::parseRWY( const char *path )
{
    std::fstream file( fdm::Path::get( path ).c_str(), std::ios_base::in );

    if ( file.is_open() )
    {
        std::string line;

        int i_line = 0;

        while ( !file.eof() )
        {
            getline( file, line );

            if ( i_line > 0 )
            {
                std::vector< std::string > cols = fdm::String::split( line, "\t" );

                line.clear();

                if ( cols.size() == 51 )
                {
                    RWY rwy;

                    rwy.he_lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 10 ) ) );
                    rwy.he_lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 12 ) ) );

                    rwy.le_lat = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 27 ) ) );
                    rwy.le_lon = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 29 ) ) );

                    rwy.true_hdg = fdm::Units::deg2rad( fdm::String::toDouble( cols.at( 44 ) ) );

                    rwy.length = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 5 ) ) );
                    rwy.width  = fdm::Units::ft2m( fdm::String::toDouble( cols.at( 6 ) ) );

                    _list_rwy.push_back( rwy );
                }
            }

            i_line++;
        }
    }
}
