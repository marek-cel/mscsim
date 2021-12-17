#!/bin/bash

################################################################################

function removeLicenceComment()
{
  echo $1
  CURRENT="(\ \* Copyright \(C\) 2021 Marek M. Cel)\n \*\n \* Permission is hereby granted, free of charge, to any person obtaining\n \* a copy of this software and associated documentation files \(the \"Software\"\),\n \* to deal in the Software without restriction, including without limitation\n \* the rights to use, copy, modify, merge, publish, distribute, sublicense,\n \* and\/or sell copies of the Software, and to permit persons to whom\n \* the Software is furnished to do so, subject to the following conditions:\n \*\n \* The above copyright notice and this permission notice shall be included\n \* in all copies or substantial portions of the Software.\n \*\n \* THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS\n \* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n \* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL\n \* THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n \* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n \* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS\n \* IN THE SOFTWARE.\n"
  REPLACE=" \* Copyright \(C\) 2021 Marek M. Cel\n \*\n \* Creative Commons Legal Code\n \*\n \* CC0 1.0 Universal\n \*\n \*     CREATIVE COMMONS CORPORATION IS NOT A LAW FIRM AND DOES NOT PROVIDE\n \*     LEGAL SERVICES. DISTRIBUTION OF THIS DOCUMENT DOES NOT CREATE AN\n \*     ATTORNEY-CLIENT RELATIONSHIP. CREATIVE COMMONS PROVIDES THIS\n \*     INFORMATION ON AN \"AS-IS\" BASIS. CREATIVE COMMONS MAKES NO WARRANTIES\n \*     REGARDING THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS\n \*     PROVIDED HEREUNDER, AND DISCLAIMS LIABILITY FOR DAMAGES RESULTING FROM\n \*     THE USE OF THIS DOCUMENT OR THE INFORMATION OR WORKS PROVIDED\n \*     HEREUNDER.\n \*\n \* Statement of Purpose\n \*\n \* The laws of most jurisdictions throughout the world automatically confer\n \* exclusive Copyright and Related Rights \(defined below\) upon the creator\n \* and subsequent owner\(s\) \(each and all, an \"owner\"\) of an original work of\n \* authorship and\/or a database \(each, a \"Work\"\).\n \*\n \* Certain owners wish to permanently relinquish those rights to a Work for\n \* the purpose of contributing to a commons of creative, cultural and\n \* scientific works \(\"Commons\"\) that the public can reliably and without fear\n \* of later claims of infringement build upon, modify, incorporate in other\n \* works, reuse and redistribute as freely as possible in any form whatsoever\n \* and for any purposes, including without limitation commercial purposes.\n \* These owners may contribute to the Commons to promote the ideal of a free\n \* culture and the further production of creative, cultural and scientific\n \* works, or to gain reputation or greater distribution for their Work in\n \* part through the use and efforts of others.\n \*\n \* For these and\/or other purposes and motivations, and without any\n \* expectation of additional consideration or compensation, the person\n \* associating CC0 with a Work \(the \"Affirmer\"\), to the extent that he or she\n \* is an owner of Copyright and Related Rights in the Work, voluntarily\n \* elects to apply CC0 to the Work and publicly distribute the Work under its\n \* terms, with knowledge of his or her Copyright and Related Rights in the\n \* Work and the meaning and intended legal effect of CC0 on those rights.\n \*\n \* 1. Copyright and Related Rights. A Work made available under CC0 may be\n \* protected by copyright and related or neighboring rights \(\"Copyright and\n \* Related Rights\"\). Copyright and Related Rights include, but are not\n \* limited to, the following:\n \*\n \*   i. the right to reproduce, adapt, distribute, perform, display,\n \*      communicate, and translate a Work;\n \*  ii. moral rights retained by the original author\(s\) and\/or performer\(s\);\n \* iii. publicity and privacy rights pertaining to a person's image or\n \*      likeness depicted in a Work;\n \*  iv. rights protecting against unfair competition in regards to a Work,\n \*      subject to the limitations in paragraph 4\(a\), below;\n \*   v. rights protecting the extraction, dissemination, use and reuse of data\n \*      in a Work;\n \*  vi. database rights \(such as those arising under Directive 96\/9\/EC of the\n \*      European Parliament and of the Council of 11 March 1996 on the legal\n \*      protection of databases, and under any national implementation\n \*      thereof, including any amended or successor version of such\n \*      directive\); and\n \* vii. other similar, equivalent or corresponding rights throughout the\n \*      world based on applicable law or treaty, and any national\n \*      implementations thereof.\n \*\n \* 2. Waiver. To the greatest extent permitted by, but not in contravention\n \* of, applicable law, Affirmer hereby overtly, fully, permanently,\n \* irrevocably and unconditionally waives, abandons, and surrenders all of\n \* Affirmer's Copyright and Related Rights and associated claims and causes\n \* of action, whether now known or unknown \(including existing as well as\n \* future claims and causes of action\), in the Work \(i\) in all territories\n \* worldwide, \(ii\) for the maximum duration provided by applicable law or\n \* treaty \(including future time extensions\), \(iii\) in any current or future\n \* medium and for any number of copies, and \(iv\) for any purpose whatsoever,\n \* including without limitation commercial, advertising or promotional\n \* purposes \(the \"Waiver\"\). Affirmer makes the Waiver for the benefit of each\n \* member of the public at large and to the detriment of Affirmer's heirs and\n \* successors, fully intending that such Waiver shall not be subject to\n \* revocation, rescission, cancellation, termination, or any other legal or\n \* equitable action to disrupt the quiet enjoyment of the Work by the public\n \* as contemplated by Affirmer's express Statement of Purpose.\n \*\n \* 3. Public License Fallback. Should any part of the Waiver for any reason\n \* be judged legally invalid or ineffective under applicable law, then the\n \* Waiver shall be preserved to the maximum extent permitted taking into\n \* account Affirmer's express Statement of Purpose. In addition, to the\n \* extent the Waiver is so judged Affirmer hereby grants to each affected\n \* person a royalty-free, non transferable, non sublicensable, non exclusive,\n \* irrevocable and unconditional license to exercise Affirmer's Copyright and\n \* Related Rights in the Work \(i\) in all territories worldwide, \(ii\) for the\n \* maximum duration provided by applicable law or treaty \(including future\n \* time extensions\), \(iii\) in any current or future medium and for any number\n \* of copies, and \(iv\) for any purpose whatsoever, including without\n \* limitation commercial, advertising or promotional purposes \(the\n \* \"License\"\). The License shall be deemed effective as of the date CC0 was\n \* applied by Affirmer to the Work. Should any part of the License for any\n \* reason be judged legally invalid or ineffective under applicable law, such\n \* partial invalidity or ineffectiveness shall not invalidate the remainder\n \* of the License, and in such case Affirmer hereby affirms that he or she\n \* will not \(i\) exercise any of his or her remaining Copyright and Related\n \* Rights in the Work or \(ii\) assert any associated claims and causes of\n \* action with respect to the Work, in either case contrary to Affirmer's\n \* express Statement of Purpose.\n \*\n \* 4. Limitations and Disclaimers.\n \*\n \*  a. No trademark or patent rights held by Affirmer are waived, abandoned,\n \*     surrendered, licensed or otherwise affected by this document.\n \*  b. Affirmer offers the Work as-is and makes no representations or\n \*     warranties of any kind concerning the Work, express, implied,\n \*     statutory or otherwise, including without limitation warranties of\n \*     title, merchantability, fitness for a particular purpose, non\n \*     infringement, or the absence of latent or other defects, accuracy, or\n \*     the present or absence of errors, whether or not discoverable, all to\n \*     the greatest extent permissible under applicable law.\n \*  c. Affirmer disclaims responsibility for clearing rights of other persons\n \*     that may apply to the Work or any use thereof, including without\n \*     limitation any person's Copyright and Related Rights in the Work.\n \*     Further, Affirmer disclaims responsibility for obtaining any necessary\n \*     consents, permissions or other rights required for any use of the\n \*     Work.\n \*  d. Affirmer understands and acknowledges that Creative Commons is not a\n \*     party to this document and has no duty or obligation with respect to\n \*     this CC0 or use of the Work.\n"
  sed -i -Ez "s/$CURRENT/$REPLACE/g" $1
}

################################################################################

function runForAllFiles()
{
  for f in $1
  do
    removeLicenceComment $f
  done
}

################################################################################

function runInSubDirectory()
{
  BASE_DIR=$(pwd)
  cd $1
  FILES=*
  runForAllFiles "${FILES[@]}"
  cd $BASE_DIR
}

################################################################################

rm -r src_cc0

svn export src src_cc0

cd src_cc0

BASE_DIR=$(pwd)
echo $BASE_DIR

FILES=*
runForAllFiles "${FILES[@]}"

runInSubDirectory cgi
runInSubDirectory cgi/hud
runInSubDirectory cgi/map
runInSubDirectory cgi/otw

runInSubDirectory fdm
runInSubDirectory fdm/auto
runInSubDirectory fdm/ctrl
runInSubDirectory fdm/models
runInSubDirectory fdm/utils
runInSubDirectory fdm/xml

runInSubDirectory fdm_aw101
runInSubDirectory fdm_c130
runInSubDirectory fdm_c172
runInSubDirectory fdm_f16
runInSubDirectory fdm_f35a
runInSubDirectory fdm_p51
runInSubDirectory fdm_pw5
runInSubDirectory fdm_r44
runInSubDirectory fdm_uh60

runInSubDirectory fdm_xf
runInSubDirectory fdm_xh

runInSubDirectory g1000
runInSubDirectory g1000/cgi
runInSubDirectory g1000/sim
runInSubDirectory g1000/utils
runInSubDirectory g1000/xml

runInSubDirectory gui

runInSubDirectory hid

runInSubDirectory nav

runInSubDirectory sfx

runInSubDirectory sim
 
 ################################################################################

read -p "Press any key to continue..." -n1 -s
