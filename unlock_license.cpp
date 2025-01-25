/*******************************************************************/
/*    Copyright (c) 2006-2015 by Spatial Corp.                     */
/*    All rights reserved.                                         */
/*    Protected by U.S. Patents 5,257,205; 5,351,196; 6,369,815;   */
/*                              5,982,378; 6,462,738; 6,941,251    */
/*    Protected by European Patents 0503642; 69220263.3            */
/*    Protected by Hong Kong Patent 1008101A                       */
/*******************************************************************/
#include "pch.h"
#include "base.hxx"

#ifdef _MSC_VER
#include "license.hxx"
#include "spa_unlock_result.hxx"


// TODO: Place the prototype to your unlock function here

void unlock_license()
{
                // TODO: Place the call to your unlock function here
    const char* unlock_str =
        "3VKKZ85TA8QRNSJT3HK3EC5NE82CGEPFQCLC2GMZTA2DVETRV8NZ2ZP8RLZZDHP7AA2CKN"
        "PUNAGZGHTLRRVZKDMTVAMKVD5TKCBRKGP7JC5Z2CP3VRJ3DE5EK8ECJAJQV8NDPN5XTXDR"
        "MGHX982KM8HH3LUDKHPE7AVZ2EHT7A2KDHMUHLZCMD5UQXDZDZJ7A3K3AZH8VRRZAG5FE8"
        "DKZ85NSC35FD8DTRGM7Y8NN8CSEQZHTEM5UDHB7E65AG5TA8Q7NCB7KGRZKQZUQHM3EA5S"
        "A3D3AZHS3GGCSU2D3HK3EQD2PE2LUDB6RET59Z8TH8QRMZ8P9VDMPC87VGRFFW8NA82KZA"
        "5NQCDFFA8XTEVL7SJJVGJ5AH5NN8CAEG5NJR6ZKCH8PADUBZJ3TRMUBUJXKA6DZZ5FQHVU"
        "MAJZKRTDJYPQVARUJYTA3REDZZMFQHRDMAPZRC6ZJYPQVARUJYPH9ATUJJNSW8QRDWHCPA"
        "DUMQP6TA5CD8P6VCJCKQTTQ8QKJC2M7AEF7QZ5R3N3ACHW3G8ZFG8NSACNVQDZ36N5KZ8U"
        "PEQKEQ5SQ863EZMBQ8KKZH5NQCPMSS2JVELC7DXNH8KKEQD7SCJDVCMTV82DNGH5K8PZPN"
        "5R3LMUEQP8KR8DMHHWS8MDKCHXPATUNCTE3XZCVZJE7RVCV8MCTA23MWJRKR5CN8PUAAUZ"
        "GAH7ARZUDJJCK8XCPCJCKRZZKNJLPA8UDZPQ9AVCKEJLKAS7JZJT98JKVNHC7A73PJP53X"
        "XDKJTNE82UEZPARLCAEQDQKVXMPC87VGRFFW8NQ8CAEQDQKRLDPC87VGRFFW8NQ8C";

    spa_unlock_result out = spa_unlock_products(unlock_str);

    if (out.get_state() == SPA_UNLOCK_PASS_WARN)
    { /* insert your warning notification code here */;
    }
    else if (out.get_state() != SPA_UNLOCK_PASS)
    { /* insert your error handling code here */;
    }
}

#else

// Non licensing platforms
void unlock_license()
{
}

#endif

