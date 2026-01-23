#pragma once

namespace lightwave {

static constexpr int PrimeTableSize = 1000;

static constexpr int Primes[PrimeTableSize] = {
    2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71,
    73, 79, 83, 89, 97, 101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151,
    157, 163, 167, 173, 179, 181, 191, 193, 197, 199, 211, 223, 227, 229, 233,
    239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293, 307, 311, 313, 317,
    331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397, 401, 409, 419,
    421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499, 503,
    509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599, 601, 607,
    613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691, 701,
    709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797, 809, 811,
    821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887, 907, 911,
    919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997, 1009, 1013, 1019,
    1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,
    1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193, 1201,
    1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291,
    1297, 1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399, 1409,
    1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487,
    1489, 1493, 1499, 1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579,
    1583, 1597, 1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667,
    1669, 1693, 1697, 1699, 1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777,
    1783, 1787, 1789, 1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877,
    1879, 1889, 1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993,
    1997, 1999, 2003, 2011, 2017, 2027, 2029, 2039, 2053, 2063, 2069, 2081, 2083,
    2087, 2089, 2099, 2111, 2113, 2129, 2131, 2137, 2141, 2143, 2153, 2161, 2179,
    2203, 2207, 2213, 2221, 2237, 2239, 2243, 2251, 2267, 2269, 2273, 2281, 2287,
    2293, 2297, 2309, 2311, 2333, 2339, 2341, 2347, 2351, 2357, 2371, 2377, 2381,
    2383, 2389, 2393, 2399, 2411, 2417, 2423, 2437, 2441, 2447, 2459, 2467, 2473,
    2477, 2503, 2521, 2531, 2539, 2543, 2549, 2551, 2557, 2579, 2591, 2593, 2609,
    2617, 2621, 2633, 2647, 2657, 2659, 2663, 2671, 2677, 2683, 2687, 2689, 2693,
    2699, 2707, 2711, 2713, 2719, 2729, 2731, 2741, 2749, 2753, 2767, 2777, 2789,
    2791, 2797, 2801, 2803, 2819, 2833, 2837, 2843, 2851, 2857, 2861, 2879, 2887,
    2897, 2903, 2909, 2917, 2927, 2939, 2953, 2957, 2963, 2969, 2971, 2999, 3001,
    3011, 3019, 3023, 3037, 3041, 3049, 3061, 3067, 3079, 3083, 3089, 3109, 3119,
    3121, 3137, 3163, 3167, 3169, 3181, 3187, 3191, 3203, 3209, 3217, 3221, 3229,
    3251, 3253, 3257, 3259, 3271, 3299, 3301, 3307, 3313, 3319, 3323, 3329, 3331,
    3343, 3347, 3359, 3361, 3371, 3373, 3389, 3391, 3407, 3413, 3433, 3449, 3457,
    3461, 3463, 3467, 3469, 3491, 3499, 3511, 3517, 3527, 3529, 3533, 3539, 3541,
    3547, 3557, 3559, 3571, 3581, 3583, 3593, 3607, 3613, 3617, 3623, 3631, 3637,
    3643, 3659, 3671, 3673, 3677, 3691, 3697, 3701, 3709, 3719, 3727, 3733, 3739,
    3761, 3767, 3769, 3779, 3793, 3797, 3803, 3821, 3823, 3833, 3847, 3851, 3853,
    3863, 3877, 3881, 3889, 3907, 3911, 3917, 3919, 3923, 3929, 3931, 3943, 3947,
    3967, 3989, 4001, 4003, 4007, 4013, 4019, 4021, 4027, 4049, 4051, 4057, 4073,
    4079, 4091, 4093, 4099, 4111, 4127, 4129, 4133, 4139, 4153, 4157, 4159, 4177,
    4201, 4211, 4217, 4219, 4229, 4231, 4241, 4243, 4253, 4259, 4261, 4271, 4273,
    4283, 4289, 4297, 4327, 4337, 4339, 4349, 4357, 4363, 4373, 4391, 4397, 4409,
    4421, 4423, 4441, 4447, 4451, 4457, 4463, 4481, 4483, 4493, 4507, 4513, 4517,
    4519, 4523, 4547, 4549, 4561, 4567, 4583, 4591, 4597, 4603, 4621, 4637, 4639,
    4643, 4649, 4651, 4657, 4663, 4673, 4679, 4691, 4703, 4721, 4723, 4729, 4733,
    4751, 4759, 4783, 4787, 4789, 4793, 4799, 4801, 4813, 4817, 4831, 4861, 4871,
    4877, 4889, 4903, 4909, 4919, 4931, 4933, 4937, 4943, 4951, 4957, 4967, 4969,
    4973, 4987, 4993, 4999, 5003, 5009, 5011, 5021, 5023, 5039, 5051, 5059, 5077,
    5081, 5087, 5099, 5101, 5107, 5113, 5119, 5147, 5153, 5167, 5171, 5179, 5189,
    5197, 5209, 5227, 5231, 5233, 5237, 5261, 5273, 5279, 5281, 5297, 5303, 5309,
    5323, 5333, 5347, 5351, 5381, 5387, 5393, 5399, 5407, 5413, 5417, 5419, 5431,
    5437, 5441, 5443, 5449, 5471, 5477, 5479, 5483, 5501, 5503, 5507, 5519, 5521,
    5527, 5531, 5557, 5563, 5569, 5573, 5581, 5591, 5623, 5639, 5641, 5647, 5651,
    5653, 5657, 5659, 5669, 5683, 5689, 5693, 5701, 5711, 5717, 5737, 5741, 5743,
    5749, 5779, 5783, 5791, 5801, 5807, 5813, 5821, 5827, 5839, 5843, 5849, 5851,
    5857, 5861, 5867, 5869, 5879, 5881, 5897, 5903, 5923, 5927, 5939, 5953, 5981,
    5987, 6007, 6011, 6029, 6037, 6043, 6047, 6053, 6067, 6073, 6079, 6089, 6091,
    6101, 6113, 6121, 6131, 6133, 6143, 6151, 6163, 6173, 6197, 6199, 6203, 6211,
    6217, 6221, 6229, 6247, 6257, 6263, 6269, 6271, 6277, 6287, 6299, 6301, 6311,
    6317, 6323, 6329, 6337, 6343, 6353, 6359, 6361, 6367, 6373, 6379, 6389, 6397,
    6421, 6427, 6449, 6451, 6469, 6473, 6481, 6491, 6521, 6529, 6547, 6551, 6553,
    6563, 6569, 6571, 6577, 6581, 6599, 6607, 6619, 6637, 6653, 6659, 6661, 6673,
    6679, 6689, 6691, 6701, 6703, 6709, 6719, 6733, 6737, 6761, 6763, 6779, 6781,
    6791, 6793, 6803, 6823, 6827, 6829, 6833, 6841, 6857, 6863, 6869, 6871, 6883,
    6899, 6907, 6911, 6917, 6947, 6949, 6959, 6961, 6967, 6971, 6977, 6983, 6991,
    6997, 7001, 7013, 7019, 7027, 7039, 7043, 7057, 7069, 7079, 7103, 7109, 7121,
    7127, 7129, 7151, 7159, 7177, 7187, 7193, 7207, 7211, 7213, 7219, 7229, 7237,
    7243, 7247, 7253, 7283, 7297, 7307, 7309, 7321, 7331, 7333, 7349, 7351, 7369,
    7393, 7411, 7417, 7433, 7451, 7457, 7459, 7477, 7481, 7487, 7489, 7499, 7507,
    7517, 7523, 7529, 7537, 7541, 7547, 7549, 7559, 7561, 7573, 7577, 7583, 7589,
    7591, 7603, 7607, 7621, 7639, 7643, 7649, 7669, 7673, 7681, 7687, 7691, 7699,
    7703, 7717, 7723, 7727, 7741, 7753, 7757, 7759, 7789, 7793, 7817, 7823, 7829,
    7841, 7853, 7867, 7873, 7877, 7879, 7883, 7901, 7907, 7919
};

const int PrimeSums[PrimeTableSize] = {
    0, 2, 5, 10, 17,
    // Subsequent prime sums
    28, 41, 58, 77, 100, 129, 160, 197, 238, 281, 328, 381, 440, 501, 568, 639,
    712, 791, 874, 963, 1060, 1161, 1264, 1371, 1480, 1593, 1720, 1851, 1988,
    2127, 2276, 2427, 2584, 2747, 2914, 3087, 3266, 3447, 3638, 3831, 4028,
    4227, 4438, 4661, 4888, 5117, 5350, 5589, 5830, 6081, 6338, 6601, 6870,
    7141, 7418, 7699, 7982, 8275, 8582, 8893, 9206, 9523, 9854, 10191, 10538,
    10887, 11240, 11599, 11966, 12339, 12718, 13101, 13490, 13887, 14288, 14697,
    15116, 15537, 15968, 16401, 16840, 17283, 17732, 18189, 18650, 19113, 19580,
    20059, 20546, 21037, 21536, 22039, 22548, 23069, 23592, 24133, 24680, 25237,
    25800, 26369, 26940, 27517, 28104, 28697, 29296, 29897, 30504, 31117, 31734,
    32353, 32984, 33625, 34268, 34915, 35568, 36227, 36888, 37561, 38238, 38921,
    39612, 40313, 41022, 41741, 42468, 43201, 43940, 44683, 45434, 46191, 46952,
    47721, 48494, 49281, 50078, 50887, 51698, 52519, 53342, 54169, 54998, 55837,
    56690, 57547, 58406, 59269, 60146, 61027, 61910, 62797, 63704, 64615, 65534,
    66463, 67400, 68341, 69288, 70241, 71208, 72179, 73156, 74139, 75130, 76127,
    77136, 78149, 79168, 80189, 81220, 82253, 83292, 84341, 85392, 86453, 87516,
    88585, 89672, 90763, 91856, 92953, 94056, 95165, 96282, 97405, 98534, 99685,
    100838, 102001, 103172, 104353, 105540, 106733, 107934, 109147, 110364,
    111587, 112816, 114047, 115284, 116533, 117792, 119069, 120348, 121631,
    122920, 124211, 125508, 126809, 128112, 129419, 130738, 132059, 133386,
    134747, 136114, 137487, 138868, 140267, 141676, 143099, 144526, 145955,
    147388, 148827, 150274, 151725, 153178, 154637, 156108, 157589, 159072,
    160559, 162048, 163541, 165040, 166551, 168074, 169605, 171148, 172697,
    174250, 175809, 177376, 178947, 180526, 182109, 183706, 185307, 186914,
    188523, 190136, 191755, 193376, 195003, 196640, 198297, 199960, 201627,
    203296, 204989, 206686, 208385, 210094, 211815, 213538, 215271, 217012,
    218759, 220512, 222271, 224048, 225831, 227618, 229407, 231208, 233019,
    234842, 236673, 238520, 240381, 242248, 244119, 245992, 247869, 249748,
    251637, 253538, 255445, 257358, 259289, 261222, 263171, 265122, 267095,
    269074, 271061, 273054, 275051, 277050, 279053, 281064, 283081, 285108,
    287137, 289176, 291229, 293292, 295361, 297442, 299525, 301612, 303701,
    305800, 307911, 310024, 312153, 314284, 316421, 318562, 320705, 322858,
    325019, 327198, 329401, 331608, 333821, 336042, 338279, 340518, 342761,
    345012, 347279, 349548, 351821, 354102, 356389, 358682, 360979, 363288,
    365599, 367932, 370271, 372612, 374959, 377310, 379667, 382038, 384415,
    386796, 389179, 391568, 393961, 396360, 398771, 401188, 403611, 406048,
    408489, 410936, 413395, 415862, 418335, 420812, 423315, 425836, 428367,
    430906, 433449, 435998, 438549, 441106, 443685, 446276, 448869, 451478,
    454095, 456716, 459349, 461996, 464653, 467312, 469975, 472646, 475323,
    478006, 480693, 483382, 486075, 488774, 491481, 494192, 496905, 499624,
    502353, 505084, 507825, 510574, 513327, 516094, 518871, 521660, 524451,
    527248, 530049, 532852, 535671, 538504, 541341, 544184, 547035, 549892,
    552753, 555632, 558519, 561416, 564319, 567228, 570145, 573072, 576011,
    578964, 581921, 584884, 587853, 590824, 593823, 596824, 599835, 602854,
    605877, 608914, 611955, 615004, 618065, 621132, 624211, 627294, 630383,
    633492, 636611, 639732, 642869, 646032, 649199, 652368, 655549, 658736,
    661927, 665130, 668339, 671556, 674777, 678006, 681257, 684510, 687767,
    691026, 694297, 697596, 700897, 704204, 707517, 710836, 714159, 717488,
    720819, 724162, 727509, 730868, 734229, 737600, 740973, 744362, 747753,
    751160, 754573, 758006, 761455, 764912, 768373, 771836, 775303, 778772,
    782263, 785762, 789273, 792790, 796317, 799846, 803379, 806918, 810459,
    814006, 817563, 821122, 824693, 828274, 831857, 835450, 839057, 842670,
    846287, 849910, 853541, 857178, 860821, 864480, 868151, 871824, 875501,
    879192, 882889, 886590, 890299, 894018, 897745, 901478, 905217, 908978,
    912745, 916514, 920293, 924086, 927883, 931686, 935507, 939330, 943163,
    947010, 950861, 954714, 958577, 962454, 966335, 970224, 974131, 978042,
    981959, 985878, 989801, 993730, 997661, 1001604, 1005551, 1009518, 1013507,
    1017508, 1021511, 1025518, 1029531, 1033550, 1037571, 1041598, 1045647,
    1049698, 1053755, 1057828, 1061907, 1065998, 1070091, 1074190, 1078301,
    1082428, 1086557, 1090690, 1094829, 1098982, 1103139, 1107298, 1111475,
    1115676, 1119887, 1124104, 1128323, 1132552, 1136783, 1141024, 1145267,
    1149520, 1153779, 1158040, 1162311, 1166584, 1170867, 1175156, 1179453,
    1183780, 1188117, 1192456, 1196805, 1201162, 1205525, 1209898, 1214289,
    1218686, 1223095, 1227516, 1231939, 1236380, 1240827, 1245278, 1249735,
    1254198, 1258679, 1263162, 1267655, 1272162, 1276675, 1281192, 1285711,
    1290234, 1294781, 1299330, 1303891, 1308458, 1313041, 1317632, 1322229,
    1326832, 1331453, 1336090, 1340729, 1345372, 1350021, 1354672, 1359329,
    1363992, 1368665, 1373344, 1378035, 1382738, 1387459, 1392182, 1396911,
    1401644, 1406395, 1411154, 1415937, 1420724, 1425513, 1430306, 1435105,
    1439906, 1444719, 1449536, 1454367, 1459228, 1464099, 1468976, 1473865,
    1478768, 1483677, 1488596, 1493527, 1498460, 1503397, 1508340, 1513291,
    1518248, 1523215, 1528184, 1533157, 1538144, 1543137, 1548136, 1553139,
    1558148, 1563159, 1568180, 1573203, 1578242, 1583293, 1588352, 1593429,
    1598510, 1603597, 1608696, 1613797, 1618904, 1624017, 1629136, 1634283,
    1639436, 1644603, 1649774, 1654953, 1660142, 1665339, 1670548, 1675775,
    1681006, 1686239, 1691476, 1696737, 1702010, 1707289, 1712570, 1717867,
    1723170, 1728479, 1733802, 1739135, 1744482, 1749833, 1755214, 1760601,
    1765994, 1771393, 1776800, 1782213, 1787630, 1793049, 1798480, 1803917,
    1809358, 1814801, 1820250, 1825721, 1831198, 1836677, 1842160, 1847661,
    1853164, 1858671, 1864190, 1869711, 1875238, 1880769, 1886326, 1891889,
    1897458, 1903031, 1908612, 1914203, 1919826, 1925465, 1931106, 1936753,
    1942404, 1948057, 1953714, 1959373, 1965042, 1970725, 1976414, 1982107,
    1987808, 1993519, 1999236, 2004973, 2010714, 2016457, 2022206, 2027985,
    2033768, 2039559, 2045360, 2051167, 2056980, 2062801, 2068628, 2074467,
    2080310, 2086159, 2092010, 2097867, 2103728, 2109595, 2115464, 2121343,
    2127224, 2133121, 2139024, 2144947, 2150874, 2156813, 2162766, 2168747,
    2174734, 2180741, 2186752, 2192781, 2198818, 2204861, 2210908, 2216961,
    2223028, 2229101, 2235180, 2241269, 2247360, 2253461, 2259574, 2265695,
    2271826, 2277959, 2284102, 2290253, 2296416, 2302589, 2308786, 2314985,
    2321188, 2327399, 2333616, 2339837, 2346066, 2352313, 2358570, 2364833,
    2371102, 2377373, 2383650, 2389937, 2396236, 2402537, 2408848, 2415165,
    2421488, 2427817, 2434154, 2440497, 2446850, 2453209, 2459570, 2465937,
    2472310, 2478689, 2485078, 2491475, 2497896, 2504323, 2510772, 2517223,
    2523692, 2530165, 2536646, 2543137, 2549658, 2556187, 2562734, 2569285,
    2575838, 2582401, 2588970, 2595541, 2602118, 2608699, 2615298, 2621905,
    2628524, 2635161, 2641814, 2648473, 2655134, 2661807, 2668486, 2675175,
    2681866, 2688567, 2695270, 2701979, 2708698, 2715431, 2722168, 2728929,
    2735692, 2742471, 2749252, 2756043, 2762836, 2769639, 2776462, 2783289,
    2790118, 2796951, 2803792, 2810649, 2817512, 2824381, 2831252, 2838135,
    2845034, 2851941, 2858852, 2865769, 2872716, 2879665, 2886624, 2893585,
    2900552, 2907523, 2914500, 2921483, 2928474, 2935471, 2942472, 2949485,
    2956504, 2963531, 2970570, 2977613, 2984670, 2991739, 2998818, 3005921,
    3013030, 3020151, 3027278, 3034407, 3041558, 3048717, 3055894, 3063081,
    3070274, 3077481, 3084692, 3091905, 3099124, 3106353, 3113590, 3120833,
    3128080, 3135333, 3142616, 3149913, 3157220, 3164529, 3171850, 3179181,
    3186514, 3193863, 3201214, 3208583, 3215976, 3223387, 3230804, 3238237,
    3245688, 3253145, 3260604, 3268081, 3275562, 3283049, 3290538, 3298037,
    3305544, 3313061, 3320584, 3328113, 3335650, 3343191, 3350738, 3358287,
    3365846, 3373407, 3380980, 3388557, 3396140, 3403729, 3411320, 3418923,
    3426530, 3434151, 3441790, 3449433, 3457082, 3464751, 3472424, 3480105,
    3487792, 3495483, 3503182, 3510885, 3518602, 3526325, 3534052, 3541793,
    3549546, 3557303, 3565062, 3572851, 3580644, 3588461, 3596284, 3604113,
    3611954, 3619807, 3627674, 3635547, 3643424, 3651303, 3659186, 3667087,
    3674994,
};

template <int base>
float ScrambledRadicalInverseSpecialized(const uint16_t *perm, uint64_t a) {
    const float invBase     = (float) 1 / (float) base;
    uint64_t reversedDigits = 0;
    float invBaseN          = 1;
    while (a) {
        uint64_t next  = a / base;
        uint64_t digit = a - next * base;
        reversedDigits = reversedDigits * base + perm[digit];
        invBaseN *= invBase;
        a = next;
    }
    return std::min(invBaseN * (reversedDigits + invBase * perm[0] / (1 - invBase)), 1.f - Epsilon);
}

float scrambledRadicalInverse(int baseIndex, uint64_t a, const uint16_t *perm) {
    switch (baseIndex) {
    case 0:
        return ScrambledRadicalInverseSpecialized<2>(perm, a);
    case 1:
        return ScrambledRadicalInverseSpecialized<3>(perm, a);
    case 2:
        return ScrambledRadicalInverseSpecialized<5>(perm, a);
    case 3:
        return ScrambledRadicalInverseSpecialized<7>(perm, a);
    // Remainder of cases for _ScrambledRadicalInverse()_
    case 4:
        return ScrambledRadicalInverseSpecialized<11>(perm, a);
    case 5:
        return ScrambledRadicalInverseSpecialized<13>(perm, a);
    case 6:
        return ScrambledRadicalInverseSpecialized<17>(perm, a);
    case 7:
        return ScrambledRadicalInverseSpecialized<19>(perm, a);
    case 8:
        return ScrambledRadicalInverseSpecialized<23>(perm, a);
    case 9:
        return ScrambledRadicalInverseSpecialized<29>(perm, a);
    case 10:
        return ScrambledRadicalInverseSpecialized<31>(perm, a);
    case 11:
        return ScrambledRadicalInverseSpecialized<37>(perm, a);
    case 12:
        return ScrambledRadicalInverseSpecialized<41>(perm, a);
    case 13:
        return ScrambledRadicalInverseSpecialized<43>(perm, a);
    case 14:
        return ScrambledRadicalInverseSpecialized<47>(perm, a);
    case 15:
        return ScrambledRadicalInverseSpecialized<53>(perm, a);
    case 16:
        return ScrambledRadicalInverseSpecialized<59>(perm, a);
    case 17:
        return ScrambledRadicalInverseSpecialized<61>(perm, a);
    case 18:
        return ScrambledRadicalInverseSpecialized<67>(perm, a);
    case 19:
        return ScrambledRadicalInverseSpecialized<71>(perm, a);
    case 20:
        return ScrambledRadicalInverseSpecialized<73>(perm, a);
    case 21:
        return ScrambledRadicalInverseSpecialized<79>(perm, a);
    case 22:
        return ScrambledRadicalInverseSpecialized<83>(perm, a);
    case 23:
        return ScrambledRadicalInverseSpecialized<89>(perm, a);
    case 24:
        return ScrambledRadicalInverseSpecialized<97>(perm, a);
    case 25:
        return ScrambledRadicalInverseSpecialized<101>(perm, a);
    case 26:
        return ScrambledRadicalInverseSpecialized<103>(perm, a);
    case 27:
        return ScrambledRadicalInverseSpecialized<107>(perm, a);
    case 28:
        return ScrambledRadicalInverseSpecialized<109>(perm, a);
    case 29:
        return ScrambledRadicalInverseSpecialized<113>(perm, a);
    case 30:
        return ScrambledRadicalInverseSpecialized<127>(perm, a);
    case 31:
        return ScrambledRadicalInverseSpecialized<131>(perm, a);
    case 32:
        return ScrambledRadicalInverseSpecialized<137>(perm, a);
    case 33:
        return ScrambledRadicalInverseSpecialized<139>(perm, a);
    case 34:
        return ScrambledRadicalInverseSpecialized<149>(perm, a);
    case 35:
        return ScrambledRadicalInverseSpecialized<151>(perm, a);
    case 36:
        return ScrambledRadicalInverseSpecialized<157>(perm, a);
    case 37:
        return ScrambledRadicalInverseSpecialized<163>(perm, a);
    case 38:
        return ScrambledRadicalInverseSpecialized<167>(perm, a);
    case 39:
        return ScrambledRadicalInverseSpecialized<173>(perm, a);
    case 40:
        return ScrambledRadicalInverseSpecialized<179>(perm, a);
    case 41:
        return ScrambledRadicalInverseSpecialized<181>(perm, a);
    case 42:
        return ScrambledRadicalInverseSpecialized<191>(perm, a);
    case 43:
        return ScrambledRadicalInverseSpecialized<193>(perm, a);
    case 44:
        return ScrambledRadicalInverseSpecialized<197>(perm, a);
    case 45:
        return ScrambledRadicalInverseSpecialized<199>(perm, a);
    case 46:
        return ScrambledRadicalInverseSpecialized<211>(perm, a);
    case 47:
        return ScrambledRadicalInverseSpecialized<223>(perm, a);
    case 48:
        return ScrambledRadicalInverseSpecialized<227>(perm, a);
    case 49:
        return ScrambledRadicalInverseSpecialized<229>(perm, a);
    case 50:
        return ScrambledRadicalInverseSpecialized<233>(perm, a);
    case 51:
        return ScrambledRadicalInverseSpecialized<239>(perm, a);
    case 52:
        return ScrambledRadicalInverseSpecialized<241>(perm, a);
    case 53:
        return ScrambledRadicalInverseSpecialized<251>(perm, a);
    case 54:
        return ScrambledRadicalInverseSpecialized<257>(perm, a);
    case 55:
        return ScrambledRadicalInverseSpecialized<263>(perm, a);
    case 56:
        return ScrambledRadicalInverseSpecialized<269>(perm, a);
    case 57:
        return ScrambledRadicalInverseSpecialized<271>(perm, a);
    case 58:
        return ScrambledRadicalInverseSpecialized<277>(perm, a);
    case 59:
        return ScrambledRadicalInverseSpecialized<281>(perm, a);
    case 60:
        return ScrambledRadicalInverseSpecialized<283>(perm, a);
    case 61:
        return ScrambledRadicalInverseSpecialized<293>(perm, a);
    case 62:
        return ScrambledRadicalInverseSpecialized<307>(perm, a);
    case 63:
        return ScrambledRadicalInverseSpecialized<311>(perm, a);
    case 64:
        return ScrambledRadicalInverseSpecialized<313>(perm, a);
    case 65:
        return ScrambledRadicalInverseSpecialized<317>(perm, a);
    case 66:
        return ScrambledRadicalInverseSpecialized<331>(perm, a);
    case 67:
        return ScrambledRadicalInverseSpecialized<337>(perm, a);
    case 68:
        return ScrambledRadicalInverseSpecialized<347>(perm, a);
    case 69:
        return ScrambledRadicalInverseSpecialized<349>(perm, a);
    case 70:
        return ScrambledRadicalInverseSpecialized<353>(perm, a);
    case 71:
        return ScrambledRadicalInverseSpecialized<359>(perm, a);
    case 72:
        return ScrambledRadicalInverseSpecialized<367>(perm, a);
    case 73:
        return ScrambledRadicalInverseSpecialized<373>(perm, a);
    case 74:
        return ScrambledRadicalInverseSpecialized<379>(perm, a);
    case 75:
        return ScrambledRadicalInverseSpecialized<383>(perm, a);
    case 76:
        return ScrambledRadicalInverseSpecialized<389>(perm, a);
    case 77:
        return ScrambledRadicalInverseSpecialized<397>(perm, a);
    case 78:
        return ScrambledRadicalInverseSpecialized<401>(perm, a);
    case 79:
        return ScrambledRadicalInverseSpecialized<409>(perm, a);
    case 80:
        return ScrambledRadicalInverseSpecialized<419>(perm, a);
    case 81:
        return ScrambledRadicalInverseSpecialized<421>(perm, a);
    case 82:
        return ScrambledRadicalInverseSpecialized<431>(perm, a);
    case 83:
        return ScrambledRadicalInverseSpecialized<433>(perm, a);
    case 84:
        return ScrambledRadicalInverseSpecialized<439>(perm, a);
    case 85:
        return ScrambledRadicalInverseSpecialized<443>(perm, a);
    case 86:
        return ScrambledRadicalInverseSpecialized<449>(perm, a);
    case 87:
        return ScrambledRadicalInverseSpecialized<457>(perm, a);
    case 88:
        return ScrambledRadicalInverseSpecialized<461>(perm, a);
    case 89:
        return ScrambledRadicalInverseSpecialized<463>(perm, a);
    case 90:
        return ScrambledRadicalInverseSpecialized<467>(perm, a);
    case 91:
        return ScrambledRadicalInverseSpecialized<479>(perm, a);
    case 92:
        return ScrambledRadicalInverseSpecialized<487>(perm, a);
    case 93:
        return ScrambledRadicalInverseSpecialized<491>(perm, a);
    case 94:
        return ScrambledRadicalInverseSpecialized<499>(perm, a);
    case 95:
        return ScrambledRadicalInverseSpecialized<503>(perm, a);
    case 96:
        return ScrambledRadicalInverseSpecialized<509>(perm, a);
    case 97:
        return ScrambledRadicalInverseSpecialized<521>(perm, a);
    case 98:
        return ScrambledRadicalInverseSpecialized<523>(perm, a);
    case 99:
        return ScrambledRadicalInverseSpecialized<541>(perm, a);
    case 100:
        return ScrambledRadicalInverseSpecialized<547>(perm, a);
    case 101:
        return ScrambledRadicalInverseSpecialized<557>(perm, a);
    case 102:
        return ScrambledRadicalInverseSpecialized<563>(perm, a);
    case 103:
        return ScrambledRadicalInverseSpecialized<569>(perm, a);
    case 104:
        return ScrambledRadicalInverseSpecialized<571>(perm, a);
    case 105:
        return ScrambledRadicalInverseSpecialized<577>(perm, a);
    case 106:
        return ScrambledRadicalInverseSpecialized<587>(perm, a);
    case 107:
        return ScrambledRadicalInverseSpecialized<593>(perm, a);
    case 108:
        return ScrambledRadicalInverseSpecialized<599>(perm, a);
    case 109:
        return ScrambledRadicalInverseSpecialized<601>(perm, a);
    case 110:
        return ScrambledRadicalInverseSpecialized<607>(perm, a);
    case 111:
        return ScrambledRadicalInverseSpecialized<613>(perm, a);
    case 112:
        return ScrambledRadicalInverseSpecialized<617>(perm, a);
    case 113:
        return ScrambledRadicalInverseSpecialized<619>(perm, a);
    case 114:
        return ScrambledRadicalInverseSpecialized<631>(perm, a);
    case 115:
        return ScrambledRadicalInverseSpecialized<641>(perm, a);
    case 116:
        return ScrambledRadicalInverseSpecialized<643>(perm, a);
    case 117:
        return ScrambledRadicalInverseSpecialized<647>(perm, a);
    case 118:
        return ScrambledRadicalInverseSpecialized<653>(perm, a);
    case 119:
        return ScrambledRadicalInverseSpecialized<659>(perm, a);
    case 120:
        return ScrambledRadicalInverseSpecialized<661>(perm, a);
    case 121:
        return ScrambledRadicalInverseSpecialized<673>(perm, a);
    case 122:
        return ScrambledRadicalInverseSpecialized<677>(perm, a);
    case 123:
        return ScrambledRadicalInverseSpecialized<683>(perm, a);
    case 124:
        return ScrambledRadicalInverseSpecialized<691>(perm, a);
    case 125:
        return ScrambledRadicalInverseSpecialized<701>(perm, a);
    case 126:
        return ScrambledRadicalInverseSpecialized<709>(perm, a);
    case 127:
        return ScrambledRadicalInverseSpecialized<719>(perm, a);
    case 128:
        return ScrambledRadicalInverseSpecialized<727>(perm, a);
    case 129:
        return ScrambledRadicalInverseSpecialized<733>(perm, a);
    case 130:
        return ScrambledRadicalInverseSpecialized<739>(perm, a);
    case 131:
        return ScrambledRadicalInverseSpecialized<743>(perm, a);
    case 132:
        return ScrambledRadicalInverseSpecialized<751>(perm, a);
    case 133:
        return ScrambledRadicalInverseSpecialized<757>(perm, a);
    case 134:
        return ScrambledRadicalInverseSpecialized<761>(perm, a);
    case 135:
        return ScrambledRadicalInverseSpecialized<769>(perm, a);
    case 136:
        return ScrambledRadicalInverseSpecialized<773>(perm, a);
    case 137:
        return ScrambledRadicalInverseSpecialized<787>(perm, a);
    case 138:
        return ScrambledRadicalInverseSpecialized<797>(perm, a);
    case 139:
        return ScrambledRadicalInverseSpecialized<809>(perm, a);
    case 140:
        return ScrambledRadicalInverseSpecialized<811>(perm, a);
    case 141:
        return ScrambledRadicalInverseSpecialized<821>(perm, a);
    case 142:
        return ScrambledRadicalInverseSpecialized<823>(perm, a);
    case 143:
        return ScrambledRadicalInverseSpecialized<827>(perm, a);
    case 144:
        return ScrambledRadicalInverseSpecialized<829>(perm, a);
    case 145:
        return ScrambledRadicalInverseSpecialized<839>(perm, a);
    case 146:
        return ScrambledRadicalInverseSpecialized<853>(perm, a);
    case 147:
        return ScrambledRadicalInverseSpecialized<857>(perm, a);
    case 148:
        return ScrambledRadicalInverseSpecialized<859>(perm, a);
    case 149:
        return ScrambledRadicalInverseSpecialized<863>(perm, a);
    case 150:
        return ScrambledRadicalInverseSpecialized<877>(perm, a);
    case 151:
        return ScrambledRadicalInverseSpecialized<881>(perm, a);
    case 152:
        return ScrambledRadicalInverseSpecialized<883>(perm, a);
    case 153:
        return ScrambledRadicalInverseSpecialized<887>(perm, a);
    case 154:
        return ScrambledRadicalInverseSpecialized<907>(perm, a);
    case 155:
        return ScrambledRadicalInverseSpecialized<911>(perm, a);
    case 156:
        return ScrambledRadicalInverseSpecialized<919>(perm, a);
    case 157:
        return ScrambledRadicalInverseSpecialized<929>(perm, a);
    case 158:
        return ScrambledRadicalInverseSpecialized<937>(perm, a);
    case 159:
        return ScrambledRadicalInverseSpecialized<941>(perm, a);
    case 160:
        return ScrambledRadicalInverseSpecialized<947>(perm, a);
    case 161:
        return ScrambledRadicalInverseSpecialized<953>(perm, a);
    case 162:
        return ScrambledRadicalInverseSpecialized<967>(perm, a);
    case 163:
        return ScrambledRadicalInverseSpecialized<971>(perm, a);
    case 164:
        return ScrambledRadicalInverseSpecialized<977>(perm, a);
    case 165:
        return ScrambledRadicalInverseSpecialized<983>(perm, a);
    case 166:
        return ScrambledRadicalInverseSpecialized<991>(perm, a);
    case 167:
        return ScrambledRadicalInverseSpecialized<997>(perm, a);
    case 168:
        return ScrambledRadicalInverseSpecialized<1009>(perm, a);
    case 169:
        return ScrambledRadicalInverseSpecialized<1013>(perm, a);
    case 170:
        return ScrambledRadicalInverseSpecialized<1019>(perm, a);
    case 171:
        return ScrambledRadicalInverseSpecialized<1021>(perm, a);
    case 172:
        return ScrambledRadicalInverseSpecialized<1031>(perm, a);
    case 173:
        return ScrambledRadicalInverseSpecialized<1033>(perm, a);
    case 174:
        return ScrambledRadicalInverseSpecialized<1039>(perm, a);
    case 175:
        return ScrambledRadicalInverseSpecialized<1049>(perm, a);
    case 176:
        return ScrambledRadicalInverseSpecialized<1051>(perm, a);
    case 177:
        return ScrambledRadicalInverseSpecialized<1061>(perm, a);
    case 178:
        return ScrambledRadicalInverseSpecialized<1063>(perm, a);
    case 179:
        return ScrambledRadicalInverseSpecialized<1069>(perm, a);
    case 180:
        return ScrambledRadicalInverseSpecialized<1087>(perm, a);
    case 181:
        return ScrambledRadicalInverseSpecialized<1091>(perm, a);
    case 182:
        return ScrambledRadicalInverseSpecialized<1093>(perm, a);
    case 183:
        return ScrambledRadicalInverseSpecialized<1097>(perm, a);
    case 184:
        return ScrambledRadicalInverseSpecialized<1103>(perm, a);
    case 185:
        return ScrambledRadicalInverseSpecialized<1109>(perm, a);
    case 186:
        return ScrambledRadicalInverseSpecialized<1117>(perm, a);
    case 187:
        return ScrambledRadicalInverseSpecialized<1123>(perm, a);
    case 188:
        return ScrambledRadicalInverseSpecialized<1129>(perm, a);
    case 189:
        return ScrambledRadicalInverseSpecialized<1151>(perm, a);
    case 190:
        return ScrambledRadicalInverseSpecialized<1153>(perm, a);
    case 191:
        return ScrambledRadicalInverseSpecialized<1163>(perm, a);
    case 192:
        return ScrambledRadicalInverseSpecialized<1171>(perm, a);
    case 193:
        return ScrambledRadicalInverseSpecialized<1181>(perm, a);
    case 194:
        return ScrambledRadicalInverseSpecialized<1187>(perm, a);
    case 195:
        return ScrambledRadicalInverseSpecialized<1193>(perm, a);
    case 196:
        return ScrambledRadicalInverseSpecialized<1201>(perm, a);
    case 197:
        return ScrambledRadicalInverseSpecialized<1213>(perm, a);
    case 198:
        return ScrambledRadicalInverseSpecialized<1217>(perm, a);
    case 199:
        return ScrambledRadicalInverseSpecialized<1223>(perm, a);
    case 200:
        return ScrambledRadicalInverseSpecialized<1229>(perm, a);
    case 201:
        return ScrambledRadicalInverseSpecialized<1231>(perm, a);
    case 202:
        return ScrambledRadicalInverseSpecialized<1237>(perm, a);
    case 203:
        return ScrambledRadicalInverseSpecialized<1249>(perm, a);
    case 204:
        return ScrambledRadicalInverseSpecialized<1259>(perm, a);
    case 205:
        return ScrambledRadicalInverseSpecialized<1277>(perm, a);
    case 206:
        return ScrambledRadicalInverseSpecialized<1279>(perm, a);
    case 207:
        return ScrambledRadicalInverseSpecialized<1283>(perm, a);
    case 208:
        return ScrambledRadicalInverseSpecialized<1289>(perm, a);
    case 209:
        return ScrambledRadicalInverseSpecialized<1291>(perm, a);
    case 210:
        return ScrambledRadicalInverseSpecialized<1297>(perm, a);
    case 211:
        return ScrambledRadicalInverseSpecialized<1301>(perm, a);
    case 212:
        return ScrambledRadicalInverseSpecialized<1303>(perm, a);
    case 213:
        return ScrambledRadicalInverseSpecialized<1307>(perm, a);
    case 214:
        return ScrambledRadicalInverseSpecialized<1319>(perm, a);
    case 215:
        return ScrambledRadicalInverseSpecialized<1321>(perm, a);
    case 216:
        return ScrambledRadicalInverseSpecialized<1327>(perm, a);
    case 217:
        return ScrambledRadicalInverseSpecialized<1361>(perm, a);
    case 218:
        return ScrambledRadicalInverseSpecialized<1367>(perm, a);
    case 219:
        return ScrambledRadicalInverseSpecialized<1373>(perm, a);
    case 220:
        return ScrambledRadicalInverseSpecialized<1381>(perm, a);
    case 221:
        return ScrambledRadicalInverseSpecialized<1399>(perm, a);
    case 222:
        return ScrambledRadicalInverseSpecialized<1409>(perm, a);
    case 223:
        return ScrambledRadicalInverseSpecialized<1423>(perm, a);
    case 224:
        return ScrambledRadicalInverseSpecialized<1427>(perm, a);
    case 225:
        return ScrambledRadicalInverseSpecialized<1429>(perm, a);
    case 226:
        return ScrambledRadicalInverseSpecialized<1433>(perm, a);
    case 227:
        return ScrambledRadicalInverseSpecialized<1439>(perm, a);
    case 228:
        return ScrambledRadicalInverseSpecialized<1447>(perm, a);
    case 229:
        return ScrambledRadicalInverseSpecialized<1451>(perm, a);
    case 230:
        return ScrambledRadicalInverseSpecialized<1453>(perm, a);
    case 231:
        return ScrambledRadicalInverseSpecialized<1459>(perm, a);
    case 232:
        return ScrambledRadicalInverseSpecialized<1471>(perm, a);
    case 233:
        return ScrambledRadicalInverseSpecialized<1481>(perm, a);
    case 234:
        return ScrambledRadicalInverseSpecialized<1483>(perm, a);
    case 235:
        return ScrambledRadicalInverseSpecialized<1487>(perm, a);
    case 236:
        return ScrambledRadicalInverseSpecialized<1489>(perm, a);
    case 237:
        return ScrambledRadicalInverseSpecialized<1493>(perm, a);
    case 238:
        return ScrambledRadicalInverseSpecialized<1499>(perm, a);
    case 239:
        return ScrambledRadicalInverseSpecialized<1511>(perm, a);
    case 240:
        return ScrambledRadicalInverseSpecialized<1523>(perm, a);
    case 241:
        return ScrambledRadicalInverseSpecialized<1531>(perm, a);
    case 242:
        return ScrambledRadicalInverseSpecialized<1543>(perm, a);
    case 243:
        return ScrambledRadicalInverseSpecialized<1549>(perm, a);
    case 244:
        return ScrambledRadicalInverseSpecialized<1553>(perm, a);
    case 245:
        return ScrambledRadicalInverseSpecialized<1559>(perm, a);
    case 246:
        return ScrambledRadicalInverseSpecialized<1567>(perm, a);
    case 247:
        return ScrambledRadicalInverseSpecialized<1571>(perm, a);
    case 248:
        return ScrambledRadicalInverseSpecialized<1579>(perm, a);
    case 249:
        return ScrambledRadicalInverseSpecialized<1583>(perm, a);
    case 250:
        return ScrambledRadicalInverseSpecialized<1597>(perm, a);
    case 251:
        return ScrambledRadicalInverseSpecialized<1601>(perm, a);
    case 252:
        return ScrambledRadicalInverseSpecialized<1607>(perm, a);
    case 253:
        return ScrambledRadicalInverseSpecialized<1609>(perm, a);
    case 254:
        return ScrambledRadicalInverseSpecialized<1613>(perm, a);
    case 255:
        return ScrambledRadicalInverseSpecialized<1619>(perm, a);
    case 256:
        return ScrambledRadicalInverseSpecialized<1621>(perm, a);
    case 257:
        return ScrambledRadicalInverseSpecialized<1627>(perm, a);
    case 258:
        return ScrambledRadicalInverseSpecialized<1637>(perm, a);
    case 259:
        return ScrambledRadicalInverseSpecialized<1657>(perm, a);
    case 260:
        return ScrambledRadicalInverseSpecialized<1663>(perm, a);
    case 261:
        return ScrambledRadicalInverseSpecialized<1667>(perm, a);
    case 262:
        return ScrambledRadicalInverseSpecialized<1669>(perm, a);
    case 263:
        return ScrambledRadicalInverseSpecialized<1693>(perm, a);
    case 264:
        return ScrambledRadicalInverseSpecialized<1697>(perm, a);
    case 265:
        return ScrambledRadicalInverseSpecialized<1699>(perm, a);
    case 266:
        return ScrambledRadicalInverseSpecialized<1709>(perm, a);
    case 267:
        return ScrambledRadicalInverseSpecialized<1721>(perm, a);
    case 268:
        return ScrambledRadicalInverseSpecialized<1723>(perm, a);
    case 269:
        return ScrambledRadicalInverseSpecialized<1733>(perm, a);
    case 270:
        return ScrambledRadicalInverseSpecialized<1741>(perm, a);
    case 271:
        return ScrambledRadicalInverseSpecialized<1747>(perm, a);
    case 272:
        return ScrambledRadicalInverseSpecialized<1753>(perm, a);
    case 273:
        return ScrambledRadicalInverseSpecialized<1759>(perm, a);
    case 274:
        return ScrambledRadicalInverseSpecialized<1777>(perm, a);
    case 275:
        return ScrambledRadicalInverseSpecialized<1783>(perm, a);
    case 276:
        return ScrambledRadicalInverseSpecialized<1787>(perm, a);
    case 277:
        return ScrambledRadicalInverseSpecialized<1789>(perm, a);
    case 278:
        return ScrambledRadicalInverseSpecialized<1801>(perm, a);
    case 279:
        return ScrambledRadicalInverseSpecialized<1811>(perm, a);
    case 280:
        return ScrambledRadicalInverseSpecialized<1823>(perm, a);
    case 281:
        return ScrambledRadicalInverseSpecialized<1831>(perm, a);
    case 282:
        return ScrambledRadicalInverseSpecialized<1847>(perm, a);
    case 283:
        return ScrambledRadicalInverseSpecialized<1861>(perm, a);
    case 284:
        return ScrambledRadicalInverseSpecialized<1867>(perm, a);
    case 285:
        return ScrambledRadicalInverseSpecialized<1871>(perm, a);
    case 286:
        return ScrambledRadicalInverseSpecialized<1873>(perm, a);
    case 287:
        return ScrambledRadicalInverseSpecialized<1877>(perm, a);
    case 288:
        return ScrambledRadicalInverseSpecialized<1879>(perm, a);
    case 289:
        return ScrambledRadicalInverseSpecialized<1889>(perm, a);
    case 290:
        return ScrambledRadicalInverseSpecialized<1901>(perm, a);
    case 291:
        return ScrambledRadicalInverseSpecialized<1907>(perm, a);
    case 292:
        return ScrambledRadicalInverseSpecialized<1913>(perm, a);
    case 293:
        return ScrambledRadicalInverseSpecialized<1931>(perm, a);
    case 294:
        return ScrambledRadicalInverseSpecialized<1933>(perm, a);
    case 295:
        return ScrambledRadicalInverseSpecialized<1949>(perm, a);
    case 296:
        return ScrambledRadicalInverseSpecialized<1951>(perm, a);
    case 297:
        return ScrambledRadicalInverseSpecialized<1973>(perm, a);
    case 298:
        return ScrambledRadicalInverseSpecialized<1979>(perm, a);
    case 299:
        return ScrambledRadicalInverseSpecialized<1987>(perm, a);
    case 300:
        return ScrambledRadicalInverseSpecialized<1993>(perm, a);
    case 301:
        return ScrambledRadicalInverseSpecialized<1997>(perm, a);
    case 302:
        return ScrambledRadicalInverseSpecialized<1999>(perm, a);
    case 303:
        return ScrambledRadicalInverseSpecialized<2003>(perm, a);
    case 304:
        return ScrambledRadicalInverseSpecialized<2011>(perm, a);
    case 305:
        return ScrambledRadicalInverseSpecialized<2017>(perm, a);
    case 306:
        return ScrambledRadicalInverseSpecialized<2027>(perm, a);
    case 307:
        return ScrambledRadicalInverseSpecialized<2029>(perm, a);
    case 308:
        return ScrambledRadicalInverseSpecialized<2039>(perm, a);
    case 309:
        return ScrambledRadicalInverseSpecialized<2053>(perm, a);
    case 310:
        return ScrambledRadicalInverseSpecialized<2063>(perm, a);
    case 311:
        return ScrambledRadicalInverseSpecialized<2069>(perm, a);
    case 312:
        return ScrambledRadicalInverseSpecialized<2081>(perm, a);
    case 313:
        return ScrambledRadicalInverseSpecialized<2083>(perm, a);
    case 314:
        return ScrambledRadicalInverseSpecialized<2087>(perm, a);
    case 315:
        return ScrambledRadicalInverseSpecialized<2089>(perm, a);
    case 316:
        return ScrambledRadicalInverseSpecialized<2099>(perm, a);
    case 317:
        return ScrambledRadicalInverseSpecialized<2111>(perm, a);
    case 318:
        return ScrambledRadicalInverseSpecialized<2113>(perm, a);
    case 319:
        return ScrambledRadicalInverseSpecialized<2129>(perm, a);
    case 320:
        return ScrambledRadicalInverseSpecialized<2131>(perm, a);
    case 321:
        return ScrambledRadicalInverseSpecialized<2137>(perm, a);
    case 322:
        return ScrambledRadicalInverseSpecialized<2141>(perm, a);
    case 323:
        return ScrambledRadicalInverseSpecialized<2143>(perm, a);
    case 324:
        return ScrambledRadicalInverseSpecialized<2153>(perm, a);
    case 325:
        return ScrambledRadicalInverseSpecialized<2161>(perm, a);
    case 326:
        return ScrambledRadicalInverseSpecialized<2179>(perm, a);
    case 327:
        return ScrambledRadicalInverseSpecialized<2203>(perm, a);
    case 328:
        return ScrambledRadicalInverseSpecialized<2207>(perm, a);
    case 329:
        return ScrambledRadicalInverseSpecialized<2213>(perm, a);
    case 330:
        return ScrambledRadicalInverseSpecialized<2221>(perm, a);
    case 331:
        return ScrambledRadicalInverseSpecialized<2237>(perm, a);
    case 332:
        return ScrambledRadicalInverseSpecialized<2239>(perm, a);
    case 333:
        return ScrambledRadicalInverseSpecialized<2243>(perm, a);
    case 334:
        return ScrambledRadicalInverseSpecialized<2251>(perm, a);
    case 335:
        return ScrambledRadicalInverseSpecialized<2267>(perm, a);
    case 336:
        return ScrambledRadicalInverseSpecialized<2269>(perm, a);
    case 337:
        return ScrambledRadicalInverseSpecialized<2273>(perm, a);
    case 338:
        return ScrambledRadicalInverseSpecialized<2281>(perm, a);
    case 339:
        return ScrambledRadicalInverseSpecialized<2287>(perm, a);
    case 340:
        return ScrambledRadicalInverseSpecialized<2293>(perm, a);
    case 341:
        return ScrambledRadicalInverseSpecialized<2297>(perm, a);
    case 342:
        return ScrambledRadicalInverseSpecialized<2309>(perm, a);
    case 343:
        return ScrambledRadicalInverseSpecialized<2311>(perm, a);
    case 344:
        return ScrambledRadicalInverseSpecialized<2333>(perm, a);
    case 345:
        return ScrambledRadicalInverseSpecialized<2339>(perm, a);
    case 346:
        return ScrambledRadicalInverseSpecialized<2341>(perm, a);
    case 347:
        return ScrambledRadicalInverseSpecialized<2347>(perm, a);
    case 348:
        return ScrambledRadicalInverseSpecialized<2351>(perm, a);
    case 349:
        return ScrambledRadicalInverseSpecialized<2357>(perm, a);
    case 350:
        return ScrambledRadicalInverseSpecialized<2371>(perm, a);
    case 351:
        return ScrambledRadicalInverseSpecialized<2377>(perm, a);
    case 352:
        return ScrambledRadicalInverseSpecialized<2381>(perm, a);
    case 353:
        return ScrambledRadicalInverseSpecialized<2383>(perm, a);
    case 354:
        return ScrambledRadicalInverseSpecialized<2389>(perm, a);
    case 355:
        return ScrambledRadicalInverseSpecialized<2393>(perm, a);
    case 356:
        return ScrambledRadicalInverseSpecialized<2399>(perm, a);
    case 357:
        return ScrambledRadicalInverseSpecialized<2411>(perm, a);
    case 358:
        return ScrambledRadicalInverseSpecialized<2417>(perm, a);
    case 359:
        return ScrambledRadicalInverseSpecialized<2423>(perm, a);
    case 360:
        return ScrambledRadicalInverseSpecialized<2437>(perm, a);
    case 361:
        return ScrambledRadicalInverseSpecialized<2441>(perm, a);
    case 362:
        return ScrambledRadicalInverseSpecialized<2447>(perm, a);
    case 363:
        return ScrambledRadicalInverseSpecialized<2459>(perm, a);
    case 364:
        return ScrambledRadicalInverseSpecialized<2467>(perm, a);
    case 365:
        return ScrambledRadicalInverseSpecialized<2473>(perm, a);
    case 366:
        return ScrambledRadicalInverseSpecialized<2477>(perm, a);
    case 367:
        return ScrambledRadicalInverseSpecialized<2503>(perm, a);
    case 368:
        return ScrambledRadicalInverseSpecialized<2521>(perm, a);
    case 369:
        return ScrambledRadicalInverseSpecialized<2531>(perm, a);
    case 370:
        return ScrambledRadicalInverseSpecialized<2539>(perm, a);
    case 371:
        return ScrambledRadicalInverseSpecialized<2543>(perm, a);
    case 372:
        return ScrambledRadicalInverseSpecialized<2549>(perm, a);
    case 373:
        return ScrambledRadicalInverseSpecialized<2551>(perm, a);
    case 374:
        return ScrambledRadicalInverseSpecialized<2557>(perm, a);
    case 375:
        return ScrambledRadicalInverseSpecialized<2579>(perm, a);
    case 376:
        return ScrambledRadicalInverseSpecialized<2591>(perm, a);
    case 377:
        return ScrambledRadicalInverseSpecialized<2593>(perm, a);
    case 378:
        return ScrambledRadicalInverseSpecialized<2609>(perm, a);
    case 379:
        return ScrambledRadicalInverseSpecialized<2617>(perm, a);
    case 380:
        return ScrambledRadicalInverseSpecialized<2621>(perm, a);
    case 381:
        return ScrambledRadicalInverseSpecialized<2633>(perm, a);
    case 382:
        return ScrambledRadicalInverseSpecialized<2647>(perm, a);
    case 383:
        return ScrambledRadicalInverseSpecialized<2657>(perm, a);
    case 384:
        return ScrambledRadicalInverseSpecialized<2659>(perm, a);
    case 385:
        return ScrambledRadicalInverseSpecialized<2663>(perm, a);
    case 386:
        return ScrambledRadicalInverseSpecialized<2671>(perm, a);
    case 387:
        return ScrambledRadicalInverseSpecialized<2677>(perm, a);
    case 388:
        return ScrambledRadicalInverseSpecialized<2683>(perm, a);
    case 389:
        return ScrambledRadicalInverseSpecialized<2687>(perm, a);
    case 390:
        return ScrambledRadicalInverseSpecialized<2689>(perm, a);
    case 391:
        return ScrambledRadicalInverseSpecialized<2693>(perm, a);
    case 392:
        return ScrambledRadicalInverseSpecialized<2699>(perm, a);
    case 393:
        return ScrambledRadicalInverseSpecialized<2707>(perm, a);
    case 394:
        return ScrambledRadicalInverseSpecialized<2711>(perm, a);
    case 395:
        return ScrambledRadicalInverseSpecialized<2713>(perm, a);
    case 396:
        return ScrambledRadicalInverseSpecialized<2719>(perm, a);
    case 397:
        return ScrambledRadicalInverseSpecialized<2729>(perm, a);
    case 398:
        return ScrambledRadicalInverseSpecialized<2731>(perm, a);
    case 399:
        return ScrambledRadicalInverseSpecialized<2741>(perm, a);
    case 400:
        return ScrambledRadicalInverseSpecialized<2749>(perm, a);
    case 401:
        return ScrambledRadicalInverseSpecialized<2753>(perm, a);
    case 402:
        return ScrambledRadicalInverseSpecialized<2767>(perm, a);
    case 403:
        return ScrambledRadicalInverseSpecialized<2777>(perm, a);
    case 404:
        return ScrambledRadicalInverseSpecialized<2789>(perm, a);
    case 405:
        return ScrambledRadicalInverseSpecialized<2791>(perm, a);
    case 406:
        return ScrambledRadicalInverseSpecialized<2797>(perm, a);
    case 407:
        return ScrambledRadicalInverseSpecialized<2801>(perm, a);
    case 408:
        return ScrambledRadicalInverseSpecialized<2803>(perm, a);
    case 409:
        return ScrambledRadicalInverseSpecialized<2819>(perm, a);
    case 410:
        return ScrambledRadicalInverseSpecialized<2833>(perm, a);
    case 411:
        return ScrambledRadicalInverseSpecialized<2837>(perm, a);
    case 412:
        return ScrambledRadicalInverseSpecialized<2843>(perm, a);
    case 413:
        return ScrambledRadicalInverseSpecialized<2851>(perm, a);
    case 414:
        return ScrambledRadicalInverseSpecialized<2857>(perm, a);
    case 415:
        return ScrambledRadicalInverseSpecialized<2861>(perm, a);
    case 416:
        return ScrambledRadicalInverseSpecialized<2879>(perm, a);
    case 417:
        return ScrambledRadicalInverseSpecialized<2887>(perm, a);
    case 418:
        return ScrambledRadicalInverseSpecialized<2897>(perm, a);
    case 419:
        return ScrambledRadicalInverseSpecialized<2903>(perm, a);
    case 420:
        return ScrambledRadicalInverseSpecialized<2909>(perm, a);
    case 421:
        return ScrambledRadicalInverseSpecialized<2917>(perm, a);
    case 422:
        return ScrambledRadicalInverseSpecialized<2927>(perm, a);
    case 423:
        return ScrambledRadicalInverseSpecialized<2939>(perm, a);
    case 424:
        return ScrambledRadicalInverseSpecialized<2953>(perm, a);
    case 425:
        return ScrambledRadicalInverseSpecialized<2957>(perm, a);
    case 426:
        return ScrambledRadicalInverseSpecialized<2963>(perm, a);
    case 427:
        return ScrambledRadicalInverseSpecialized<2969>(perm, a);
    case 428:
        return ScrambledRadicalInverseSpecialized<2971>(perm, a);
    case 429:
        return ScrambledRadicalInverseSpecialized<2999>(perm, a);
    case 430:
        return ScrambledRadicalInverseSpecialized<3001>(perm, a);
    case 431:
        return ScrambledRadicalInverseSpecialized<3011>(perm, a);
    case 432:
        return ScrambledRadicalInverseSpecialized<3019>(perm, a);
    case 433:
        return ScrambledRadicalInverseSpecialized<3023>(perm, a);
    case 434:
        return ScrambledRadicalInverseSpecialized<3037>(perm, a);
    case 435:
        return ScrambledRadicalInverseSpecialized<3041>(perm, a);
    case 436:
        return ScrambledRadicalInverseSpecialized<3049>(perm, a);
    case 437:
        return ScrambledRadicalInverseSpecialized<3061>(perm, a);
    case 438:
        return ScrambledRadicalInverseSpecialized<3067>(perm, a);
    case 439:
        return ScrambledRadicalInverseSpecialized<3079>(perm, a);
    case 440:
        return ScrambledRadicalInverseSpecialized<3083>(perm, a);
    case 441:
        return ScrambledRadicalInverseSpecialized<3089>(perm, a);
    case 442:
        return ScrambledRadicalInverseSpecialized<3109>(perm, a);
    case 443:
        return ScrambledRadicalInverseSpecialized<3119>(perm, a);
    case 444:
        return ScrambledRadicalInverseSpecialized<3121>(perm, a);
    case 445:
        return ScrambledRadicalInverseSpecialized<3137>(perm, a);
    case 446:
        return ScrambledRadicalInverseSpecialized<3163>(perm, a);
    case 447:
        return ScrambledRadicalInverseSpecialized<3167>(perm, a);
    case 448:
        return ScrambledRadicalInverseSpecialized<3169>(perm, a);
    case 449:
        return ScrambledRadicalInverseSpecialized<3181>(perm, a);
    case 450:
        return ScrambledRadicalInverseSpecialized<3187>(perm, a);
    case 451:
        return ScrambledRadicalInverseSpecialized<3191>(perm, a);
    case 452:
        return ScrambledRadicalInverseSpecialized<3203>(perm, a);
    case 453:
        return ScrambledRadicalInverseSpecialized<3209>(perm, a);
    case 454:
        return ScrambledRadicalInverseSpecialized<3217>(perm, a);
    case 455:
        return ScrambledRadicalInverseSpecialized<3221>(perm, a);
    case 456:
        return ScrambledRadicalInverseSpecialized<3229>(perm, a);
    case 457:
        return ScrambledRadicalInverseSpecialized<3251>(perm, a);
    case 458:
        return ScrambledRadicalInverseSpecialized<3253>(perm, a);
    case 459:
        return ScrambledRadicalInverseSpecialized<3257>(perm, a);
    case 460:
        return ScrambledRadicalInverseSpecialized<3259>(perm, a);
    case 461:
        return ScrambledRadicalInverseSpecialized<3271>(perm, a);
    case 462:
        return ScrambledRadicalInverseSpecialized<3299>(perm, a);
    case 463:
        return ScrambledRadicalInverseSpecialized<3301>(perm, a);
    case 464:
        return ScrambledRadicalInverseSpecialized<3307>(perm, a);
    case 465:
        return ScrambledRadicalInverseSpecialized<3313>(perm, a);
    case 466:
        return ScrambledRadicalInverseSpecialized<3319>(perm, a);
    case 467:
        return ScrambledRadicalInverseSpecialized<3323>(perm, a);
    case 468:
        return ScrambledRadicalInverseSpecialized<3329>(perm, a);
    case 469:
        return ScrambledRadicalInverseSpecialized<3331>(perm, a);
    case 470:
        return ScrambledRadicalInverseSpecialized<3343>(perm, a);
    case 471:
        return ScrambledRadicalInverseSpecialized<3347>(perm, a);
    case 472:
        return ScrambledRadicalInverseSpecialized<3359>(perm, a);
    case 473:
        return ScrambledRadicalInverseSpecialized<3361>(perm, a);
    case 474:
        return ScrambledRadicalInverseSpecialized<3371>(perm, a);
    case 475:
        return ScrambledRadicalInverseSpecialized<3373>(perm, a);
    case 476:
        return ScrambledRadicalInverseSpecialized<3389>(perm, a);
    case 477:
        return ScrambledRadicalInverseSpecialized<3391>(perm, a);
    case 478:
        return ScrambledRadicalInverseSpecialized<3407>(perm, a);
    case 479:
        return ScrambledRadicalInverseSpecialized<3413>(perm, a);
    case 480:
        return ScrambledRadicalInverseSpecialized<3433>(perm, a);
    case 481:
        return ScrambledRadicalInverseSpecialized<3449>(perm, a);
    case 482:
        return ScrambledRadicalInverseSpecialized<3457>(perm, a);
    case 483:
        return ScrambledRadicalInverseSpecialized<3461>(perm, a);
    case 484:
        return ScrambledRadicalInverseSpecialized<3463>(perm, a);
    case 485:
        return ScrambledRadicalInverseSpecialized<3467>(perm, a);
    case 486:
        return ScrambledRadicalInverseSpecialized<3469>(perm, a);
    case 487:
        return ScrambledRadicalInverseSpecialized<3491>(perm, a);
    case 488:
        return ScrambledRadicalInverseSpecialized<3499>(perm, a);
    case 489:
        return ScrambledRadicalInverseSpecialized<3511>(perm, a);
    case 490:
        return ScrambledRadicalInverseSpecialized<3517>(perm, a);
    case 491:
        return ScrambledRadicalInverseSpecialized<3527>(perm, a);
    case 492:
        return ScrambledRadicalInverseSpecialized<3529>(perm, a);
    case 493:
        return ScrambledRadicalInverseSpecialized<3533>(perm, a);
    case 494:
        return ScrambledRadicalInverseSpecialized<3539>(perm, a);
    case 495:
        return ScrambledRadicalInverseSpecialized<3541>(perm, a);
    case 496:
        return ScrambledRadicalInverseSpecialized<3547>(perm, a);
    case 497:
        return ScrambledRadicalInverseSpecialized<3557>(perm, a);
    case 498:
        return ScrambledRadicalInverseSpecialized<3559>(perm, a);
    case 499:
        return ScrambledRadicalInverseSpecialized<3571>(perm, a);
    case 500:
        return ScrambledRadicalInverseSpecialized<3581>(perm, a);
    case 501:
        return ScrambledRadicalInverseSpecialized<3583>(perm, a);
    case 502:
        return ScrambledRadicalInverseSpecialized<3593>(perm, a);
    case 503:
        return ScrambledRadicalInverseSpecialized<3607>(perm, a);
    case 504:
        return ScrambledRadicalInverseSpecialized<3613>(perm, a);
    case 505:
        return ScrambledRadicalInverseSpecialized<3617>(perm, a);
    case 506:
        return ScrambledRadicalInverseSpecialized<3623>(perm, a);
    case 507:
        return ScrambledRadicalInverseSpecialized<3631>(perm, a);
    case 508:
        return ScrambledRadicalInverseSpecialized<3637>(perm, a);
    case 509:
        return ScrambledRadicalInverseSpecialized<3643>(perm, a);
    case 510:
        return ScrambledRadicalInverseSpecialized<3659>(perm, a);
    case 511:
        return ScrambledRadicalInverseSpecialized<3671>(perm, a);
    case 512:
        return ScrambledRadicalInverseSpecialized<3673>(perm, a);
    case 513:
        return ScrambledRadicalInverseSpecialized<3677>(perm, a);
    case 514:
        return ScrambledRadicalInverseSpecialized<3691>(perm, a);
    case 515:
        return ScrambledRadicalInverseSpecialized<3697>(perm, a);
    case 516:
        return ScrambledRadicalInverseSpecialized<3701>(perm, a);
    case 517:
        return ScrambledRadicalInverseSpecialized<3709>(perm, a);
    case 518:
        return ScrambledRadicalInverseSpecialized<3719>(perm, a);
    case 519:
        return ScrambledRadicalInverseSpecialized<3727>(perm, a);
    case 520:
        return ScrambledRadicalInverseSpecialized<3733>(perm, a);
    case 521:
        return ScrambledRadicalInverseSpecialized<3739>(perm, a);
    case 522:
        return ScrambledRadicalInverseSpecialized<3761>(perm, a);
    case 523:
        return ScrambledRadicalInverseSpecialized<3767>(perm, a);
    case 524:
        return ScrambledRadicalInverseSpecialized<3769>(perm, a);
    case 525:
        return ScrambledRadicalInverseSpecialized<3779>(perm, a);
    case 526:
        return ScrambledRadicalInverseSpecialized<3793>(perm, a);
    case 527:
        return ScrambledRadicalInverseSpecialized<3797>(perm, a);
    case 528:
        return ScrambledRadicalInverseSpecialized<3803>(perm, a);
    case 529:
        return ScrambledRadicalInverseSpecialized<3821>(perm, a);
    case 530:
        return ScrambledRadicalInverseSpecialized<3823>(perm, a);
    case 531:
        return ScrambledRadicalInverseSpecialized<3833>(perm, a);
    case 532:
        return ScrambledRadicalInverseSpecialized<3847>(perm, a);
    case 533:
        return ScrambledRadicalInverseSpecialized<3851>(perm, a);
    case 534:
        return ScrambledRadicalInverseSpecialized<3853>(perm, a);
    case 535:
        return ScrambledRadicalInverseSpecialized<3863>(perm, a);
    case 536:
        return ScrambledRadicalInverseSpecialized<3877>(perm, a);
    case 537:
        return ScrambledRadicalInverseSpecialized<3881>(perm, a);
    case 538:
        return ScrambledRadicalInverseSpecialized<3889>(perm, a);
    case 539:
        return ScrambledRadicalInverseSpecialized<3907>(perm, a);
    case 540:
        return ScrambledRadicalInverseSpecialized<3911>(perm, a);
    case 541:
        return ScrambledRadicalInverseSpecialized<3917>(perm, a);
    case 542:
        return ScrambledRadicalInverseSpecialized<3919>(perm, a);
    case 543:
        return ScrambledRadicalInverseSpecialized<3923>(perm, a);
    case 544:
        return ScrambledRadicalInverseSpecialized<3929>(perm, a);
    case 545:
        return ScrambledRadicalInverseSpecialized<3931>(perm, a);
    case 546:
        return ScrambledRadicalInverseSpecialized<3943>(perm, a);
    case 547:
        return ScrambledRadicalInverseSpecialized<3947>(perm, a);
    case 548:
        return ScrambledRadicalInverseSpecialized<3967>(perm, a);
    case 549:
        return ScrambledRadicalInverseSpecialized<3989>(perm, a);
    case 550:
        return ScrambledRadicalInverseSpecialized<4001>(perm, a);
    case 551:
        return ScrambledRadicalInverseSpecialized<4003>(perm, a);
    case 552:
        return ScrambledRadicalInverseSpecialized<4007>(perm, a);
    case 553:
        return ScrambledRadicalInverseSpecialized<4013>(perm, a);
    case 554:
        return ScrambledRadicalInverseSpecialized<4019>(perm, a);
    case 555:
        return ScrambledRadicalInverseSpecialized<4021>(perm, a);
    case 556:
        return ScrambledRadicalInverseSpecialized<4027>(perm, a);
    case 557:
        return ScrambledRadicalInverseSpecialized<4049>(perm, a);
    case 558:
        return ScrambledRadicalInverseSpecialized<4051>(perm, a);
    case 559:
        return ScrambledRadicalInverseSpecialized<4057>(perm, a);
    case 560:
        return ScrambledRadicalInverseSpecialized<4073>(perm, a);
    case 561:
        return ScrambledRadicalInverseSpecialized<4079>(perm, a);
    case 562:
        return ScrambledRadicalInverseSpecialized<4091>(perm, a);
    case 563:
        return ScrambledRadicalInverseSpecialized<4093>(perm, a);
    case 564:
        return ScrambledRadicalInverseSpecialized<4099>(perm, a);
    case 565:
        return ScrambledRadicalInverseSpecialized<4111>(perm, a);
    case 566:
        return ScrambledRadicalInverseSpecialized<4127>(perm, a);
    case 567:
        return ScrambledRadicalInverseSpecialized<4129>(perm, a);
    case 568:
        return ScrambledRadicalInverseSpecialized<4133>(perm, a);
    case 569:
        return ScrambledRadicalInverseSpecialized<4139>(perm, a);
    case 570:
        return ScrambledRadicalInverseSpecialized<4153>(perm, a);
    case 571:
        return ScrambledRadicalInverseSpecialized<4157>(perm, a);
    case 572:
        return ScrambledRadicalInverseSpecialized<4159>(perm, a);
    case 573:
        return ScrambledRadicalInverseSpecialized<4177>(perm, a);
    case 574:
        return ScrambledRadicalInverseSpecialized<4201>(perm, a);
    case 575:
        return ScrambledRadicalInverseSpecialized<4211>(perm, a);
    case 576:
        return ScrambledRadicalInverseSpecialized<4217>(perm, a);
    case 577:
        return ScrambledRadicalInverseSpecialized<4219>(perm, a);
    case 578:
        return ScrambledRadicalInverseSpecialized<4229>(perm, a);
    case 579:
        return ScrambledRadicalInverseSpecialized<4231>(perm, a);
    case 580:
        return ScrambledRadicalInverseSpecialized<4241>(perm, a);
    case 581:
        return ScrambledRadicalInverseSpecialized<4243>(perm, a);
    case 582:
        return ScrambledRadicalInverseSpecialized<4253>(perm, a);
    case 583:
        return ScrambledRadicalInverseSpecialized<4259>(perm, a);
    case 584:
        return ScrambledRadicalInverseSpecialized<4261>(perm, a);
    case 585:
        return ScrambledRadicalInverseSpecialized<4271>(perm, a);
    case 586:
        return ScrambledRadicalInverseSpecialized<4273>(perm, a);
    case 587:
        return ScrambledRadicalInverseSpecialized<4283>(perm, a);
    case 588:
        return ScrambledRadicalInverseSpecialized<4289>(perm, a);
    case 589:
        return ScrambledRadicalInverseSpecialized<4297>(perm, a);
    case 590:
        return ScrambledRadicalInverseSpecialized<4327>(perm, a);
    case 591:
        return ScrambledRadicalInverseSpecialized<4337>(perm, a);
    case 592:
        return ScrambledRadicalInverseSpecialized<4339>(perm, a);
    case 593:
        return ScrambledRadicalInverseSpecialized<4349>(perm, a);
    case 594:
        return ScrambledRadicalInverseSpecialized<4357>(perm, a);
    case 595:
        return ScrambledRadicalInverseSpecialized<4363>(perm, a);
    case 596:
        return ScrambledRadicalInverseSpecialized<4373>(perm, a);
    case 597:
        return ScrambledRadicalInverseSpecialized<4391>(perm, a);
    case 598:
        return ScrambledRadicalInverseSpecialized<4397>(perm, a);
    case 599:
        return ScrambledRadicalInverseSpecialized<4409>(perm, a);
    case 600:
        return ScrambledRadicalInverseSpecialized<4421>(perm, a);
    case 601:
        return ScrambledRadicalInverseSpecialized<4423>(perm, a);
    case 602:
        return ScrambledRadicalInverseSpecialized<4441>(perm, a);
    case 603:
        return ScrambledRadicalInverseSpecialized<4447>(perm, a);
    case 604:
        return ScrambledRadicalInverseSpecialized<4451>(perm, a);
    case 605:
        return ScrambledRadicalInverseSpecialized<4457>(perm, a);
    case 606:
        return ScrambledRadicalInverseSpecialized<4463>(perm, a);
    case 607:
        return ScrambledRadicalInverseSpecialized<4481>(perm, a);
    case 608:
        return ScrambledRadicalInverseSpecialized<4483>(perm, a);
    case 609:
        return ScrambledRadicalInverseSpecialized<4493>(perm, a);
    case 610:
        return ScrambledRadicalInverseSpecialized<4507>(perm, a);
    case 611:
        return ScrambledRadicalInverseSpecialized<4513>(perm, a);
    case 612:
        return ScrambledRadicalInverseSpecialized<4517>(perm, a);
    case 613:
        return ScrambledRadicalInverseSpecialized<4519>(perm, a);
    case 614:
        return ScrambledRadicalInverseSpecialized<4523>(perm, a);
    case 615:
        return ScrambledRadicalInverseSpecialized<4547>(perm, a);
    case 616:
        return ScrambledRadicalInverseSpecialized<4549>(perm, a);
    case 617:
        return ScrambledRadicalInverseSpecialized<4561>(perm, a);
    case 618:
        return ScrambledRadicalInverseSpecialized<4567>(perm, a);
    case 619:
        return ScrambledRadicalInverseSpecialized<4583>(perm, a);
    case 620:
        return ScrambledRadicalInverseSpecialized<4591>(perm, a);
    case 621:
        return ScrambledRadicalInverseSpecialized<4597>(perm, a);
    case 622:
        return ScrambledRadicalInverseSpecialized<4603>(perm, a);
    case 623:
        return ScrambledRadicalInverseSpecialized<4621>(perm, a);
    case 624:
        return ScrambledRadicalInverseSpecialized<4637>(perm, a);
    case 625:
        return ScrambledRadicalInverseSpecialized<4639>(perm, a);
    case 626:
        return ScrambledRadicalInverseSpecialized<4643>(perm, a);
    case 627:
        return ScrambledRadicalInverseSpecialized<4649>(perm, a);
    case 628:
        return ScrambledRadicalInverseSpecialized<4651>(perm, a);
    case 629:
        return ScrambledRadicalInverseSpecialized<4657>(perm, a);
    case 630:
        return ScrambledRadicalInverseSpecialized<4663>(perm, a);
    case 631:
        return ScrambledRadicalInverseSpecialized<4673>(perm, a);
    case 632:
        return ScrambledRadicalInverseSpecialized<4679>(perm, a);
    case 633:
        return ScrambledRadicalInverseSpecialized<4691>(perm, a);
    case 634:
        return ScrambledRadicalInverseSpecialized<4703>(perm, a);
    case 635:
        return ScrambledRadicalInverseSpecialized<4721>(perm, a);
    case 636:
        return ScrambledRadicalInverseSpecialized<4723>(perm, a);
    case 637:
        return ScrambledRadicalInverseSpecialized<4729>(perm, a);
    case 638:
        return ScrambledRadicalInverseSpecialized<4733>(perm, a);
    case 639:
        return ScrambledRadicalInverseSpecialized<4751>(perm, a);
    case 640:
        return ScrambledRadicalInverseSpecialized<4759>(perm, a);
    case 641:
        return ScrambledRadicalInverseSpecialized<4783>(perm, a);
    case 642:
        return ScrambledRadicalInverseSpecialized<4787>(perm, a);
    case 643:
        return ScrambledRadicalInverseSpecialized<4789>(perm, a);
    case 644:
        return ScrambledRadicalInverseSpecialized<4793>(perm, a);
    case 645:
        return ScrambledRadicalInverseSpecialized<4799>(perm, a);
    case 646:
        return ScrambledRadicalInverseSpecialized<4801>(perm, a);
    case 647:
        return ScrambledRadicalInverseSpecialized<4813>(perm, a);
    case 648:
        return ScrambledRadicalInverseSpecialized<4817>(perm, a);
    case 649:
        return ScrambledRadicalInverseSpecialized<4831>(perm, a);
    case 650:
        return ScrambledRadicalInverseSpecialized<4861>(perm, a);
    case 651:
        return ScrambledRadicalInverseSpecialized<4871>(perm, a);
    case 652:
        return ScrambledRadicalInverseSpecialized<4877>(perm, a);
    case 653:
        return ScrambledRadicalInverseSpecialized<4889>(perm, a);
    case 654:
        return ScrambledRadicalInverseSpecialized<4903>(perm, a);
    case 655:
        return ScrambledRadicalInverseSpecialized<4909>(perm, a);
    case 656:
        return ScrambledRadicalInverseSpecialized<4919>(perm, a);
    case 657:
        return ScrambledRadicalInverseSpecialized<4931>(perm, a);
    case 658:
        return ScrambledRadicalInverseSpecialized<4933>(perm, a);
    case 659:
        return ScrambledRadicalInverseSpecialized<4937>(perm, a);
    case 660:
        return ScrambledRadicalInverseSpecialized<4943>(perm, a);
    case 661:
        return ScrambledRadicalInverseSpecialized<4951>(perm, a);
    case 662:
        return ScrambledRadicalInverseSpecialized<4957>(perm, a);
    case 663:
        return ScrambledRadicalInverseSpecialized<4967>(perm, a);
    case 664:
        return ScrambledRadicalInverseSpecialized<4969>(perm, a);
    case 665:
        return ScrambledRadicalInverseSpecialized<4973>(perm, a);
    case 666:
        return ScrambledRadicalInverseSpecialized<4987>(perm, a);
    case 667:
        return ScrambledRadicalInverseSpecialized<4993>(perm, a);
    case 668:
        return ScrambledRadicalInverseSpecialized<4999>(perm, a);
    case 669:
        return ScrambledRadicalInverseSpecialized<5003>(perm, a);
    case 670:
        return ScrambledRadicalInverseSpecialized<5009>(perm, a);
    case 671:
        return ScrambledRadicalInverseSpecialized<5011>(perm, a);
    case 672:
        return ScrambledRadicalInverseSpecialized<5021>(perm, a);
    case 673:
        return ScrambledRadicalInverseSpecialized<5023>(perm, a);
    case 674:
        return ScrambledRadicalInverseSpecialized<5039>(perm, a);
    case 675:
        return ScrambledRadicalInverseSpecialized<5051>(perm, a);
    case 676:
        return ScrambledRadicalInverseSpecialized<5059>(perm, a);
    case 677:
        return ScrambledRadicalInverseSpecialized<5077>(perm, a);
    case 678:
        return ScrambledRadicalInverseSpecialized<5081>(perm, a);
    case 679:
        return ScrambledRadicalInverseSpecialized<5087>(perm, a);
    case 680:
        return ScrambledRadicalInverseSpecialized<5099>(perm, a);
    case 681:
        return ScrambledRadicalInverseSpecialized<5101>(perm, a);
    case 682:
        return ScrambledRadicalInverseSpecialized<5107>(perm, a);
    case 683:
        return ScrambledRadicalInverseSpecialized<5113>(perm, a);
    case 684:
        return ScrambledRadicalInverseSpecialized<5119>(perm, a);
    case 685:
        return ScrambledRadicalInverseSpecialized<5147>(perm, a);
    case 686:
        return ScrambledRadicalInverseSpecialized<5153>(perm, a);
    case 687:
        return ScrambledRadicalInverseSpecialized<5167>(perm, a);
    case 688:
        return ScrambledRadicalInverseSpecialized<5171>(perm, a);
    case 689:
        return ScrambledRadicalInverseSpecialized<5179>(perm, a);
    case 690:
        return ScrambledRadicalInverseSpecialized<5189>(perm, a);
    case 691:
        return ScrambledRadicalInverseSpecialized<5197>(perm, a);
    case 692:
        return ScrambledRadicalInverseSpecialized<5209>(perm, a);
    case 693:
        return ScrambledRadicalInverseSpecialized<5227>(perm, a);
    case 694:
        return ScrambledRadicalInverseSpecialized<5231>(perm, a);
    case 695:
        return ScrambledRadicalInverseSpecialized<5233>(perm, a);
    case 696:
        return ScrambledRadicalInverseSpecialized<5237>(perm, a);
    case 697:
        return ScrambledRadicalInverseSpecialized<5261>(perm, a);
    case 698:
        return ScrambledRadicalInverseSpecialized<5273>(perm, a);
    case 699:
        return ScrambledRadicalInverseSpecialized<5279>(perm, a);
    case 700:
        return ScrambledRadicalInverseSpecialized<5281>(perm, a);
    case 701:
        return ScrambledRadicalInverseSpecialized<5297>(perm, a);
    case 702:
        return ScrambledRadicalInverseSpecialized<5303>(perm, a);
    case 703:
        return ScrambledRadicalInverseSpecialized<5309>(perm, a);
    case 704:
        return ScrambledRadicalInverseSpecialized<5323>(perm, a);
    case 705:
        return ScrambledRadicalInverseSpecialized<5333>(perm, a);
    case 706:
        return ScrambledRadicalInverseSpecialized<5347>(perm, a);
    case 707:
        return ScrambledRadicalInverseSpecialized<5351>(perm, a);
    case 708:
        return ScrambledRadicalInverseSpecialized<5381>(perm, a);
    case 709:
        return ScrambledRadicalInverseSpecialized<5387>(perm, a);
    case 710:
        return ScrambledRadicalInverseSpecialized<5393>(perm, a);
    case 711:
        return ScrambledRadicalInverseSpecialized<5399>(perm, a);
    case 712:
        return ScrambledRadicalInverseSpecialized<5407>(perm, a);
    case 713:
        return ScrambledRadicalInverseSpecialized<5413>(perm, a);
    case 714:
        return ScrambledRadicalInverseSpecialized<5417>(perm, a);
    case 715:
        return ScrambledRadicalInverseSpecialized<5419>(perm, a);
    case 716:
        return ScrambledRadicalInverseSpecialized<5431>(perm, a);
    case 717:
        return ScrambledRadicalInverseSpecialized<5437>(perm, a);
    case 718:
        return ScrambledRadicalInverseSpecialized<5441>(perm, a);
    case 719:
        return ScrambledRadicalInverseSpecialized<5443>(perm, a);
    case 720:
        return ScrambledRadicalInverseSpecialized<5449>(perm, a);
    case 721:
        return ScrambledRadicalInverseSpecialized<5471>(perm, a);
    case 722:
        return ScrambledRadicalInverseSpecialized<5477>(perm, a);
    case 723:
        return ScrambledRadicalInverseSpecialized<5479>(perm, a);
    case 724:
        return ScrambledRadicalInverseSpecialized<5483>(perm, a);
    case 725:
        return ScrambledRadicalInverseSpecialized<5501>(perm, a);
    case 726:
        return ScrambledRadicalInverseSpecialized<5503>(perm, a);
    case 727:
        return ScrambledRadicalInverseSpecialized<5507>(perm, a);
    case 728:
        return ScrambledRadicalInverseSpecialized<5519>(perm, a);
    case 729:
        return ScrambledRadicalInverseSpecialized<5521>(perm, a);
    case 730:
        return ScrambledRadicalInverseSpecialized<5527>(perm, a);
    case 731:
        return ScrambledRadicalInverseSpecialized<5531>(perm, a);
    case 732:
        return ScrambledRadicalInverseSpecialized<5557>(perm, a);
    case 733:
        return ScrambledRadicalInverseSpecialized<5563>(perm, a);
    case 734:
        return ScrambledRadicalInverseSpecialized<5569>(perm, a);
    case 735:
        return ScrambledRadicalInverseSpecialized<5573>(perm, a);
    case 736:
        return ScrambledRadicalInverseSpecialized<5581>(perm, a);
    case 737:
        return ScrambledRadicalInverseSpecialized<5591>(perm, a);
    case 738:
        return ScrambledRadicalInverseSpecialized<5623>(perm, a);
    case 739:
        return ScrambledRadicalInverseSpecialized<5639>(perm, a);
    case 740:
        return ScrambledRadicalInverseSpecialized<5641>(perm, a);
    case 741:
        return ScrambledRadicalInverseSpecialized<5647>(perm, a);
    case 742:
        return ScrambledRadicalInverseSpecialized<5651>(perm, a);
    case 743:
        return ScrambledRadicalInverseSpecialized<5653>(perm, a);
    case 744:
        return ScrambledRadicalInverseSpecialized<5657>(perm, a);
    case 745:
        return ScrambledRadicalInverseSpecialized<5659>(perm, a);
    case 746:
        return ScrambledRadicalInverseSpecialized<5669>(perm, a);
    case 747:
        return ScrambledRadicalInverseSpecialized<5683>(perm, a);
    case 748:
        return ScrambledRadicalInverseSpecialized<5689>(perm, a);
    case 749:
        return ScrambledRadicalInverseSpecialized<5693>(perm, a);
    case 750:
        return ScrambledRadicalInverseSpecialized<5701>(perm, a);
    case 751:
        return ScrambledRadicalInverseSpecialized<5711>(perm, a);
    case 752:
        return ScrambledRadicalInverseSpecialized<5717>(perm, a);
    case 753:
        return ScrambledRadicalInverseSpecialized<5737>(perm, a);
    case 754:
        return ScrambledRadicalInverseSpecialized<5741>(perm, a);
    case 755:
        return ScrambledRadicalInverseSpecialized<5743>(perm, a);
    case 756:
        return ScrambledRadicalInverseSpecialized<5749>(perm, a);
    case 757:
        return ScrambledRadicalInverseSpecialized<5779>(perm, a);
    case 758:
        return ScrambledRadicalInverseSpecialized<5783>(perm, a);
    case 759:
        return ScrambledRadicalInverseSpecialized<5791>(perm, a);
    case 760:
        return ScrambledRadicalInverseSpecialized<5801>(perm, a);
    case 761:
        return ScrambledRadicalInverseSpecialized<5807>(perm, a);
    case 762:
        return ScrambledRadicalInverseSpecialized<5813>(perm, a);
    case 763:
        return ScrambledRadicalInverseSpecialized<5821>(perm, a);
    case 764:
        return ScrambledRadicalInverseSpecialized<5827>(perm, a);
    case 765:
        return ScrambledRadicalInverseSpecialized<5839>(perm, a);
    case 766:
        return ScrambledRadicalInverseSpecialized<5843>(perm, a);
    case 767:
        return ScrambledRadicalInverseSpecialized<5849>(perm, a);
    case 768:
        return ScrambledRadicalInverseSpecialized<5851>(perm, a);
    case 769:
        return ScrambledRadicalInverseSpecialized<5857>(perm, a);
    case 770:
        return ScrambledRadicalInverseSpecialized<5861>(perm, a);
    case 771:
        return ScrambledRadicalInverseSpecialized<5867>(perm, a);
    case 772:
        return ScrambledRadicalInverseSpecialized<5869>(perm, a);
    case 773:
        return ScrambledRadicalInverseSpecialized<5879>(perm, a);
    case 774:
        return ScrambledRadicalInverseSpecialized<5881>(perm, a);
    case 775:
        return ScrambledRadicalInverseSpecialized<5897>(perm, a);
    case 776:
        return ScrambledRadicalInverseSpecialized<5903>(perm, a);
    case 777:
        return ScrambledRadicalInverseSpecialized<5923>(perm, a);
    case 778:
        return ScrambledRadicalInverseSpecialized<5927>(perm, a);
    case 779:
        return ScrambledRadicalInverseSpecialized<5939>(perm, a);
    case 780:
        return ScrambledRadicalInverseSpecialized<5953>(perm, a);
    case 781:
        return ScrambledRadicalInverseSpecialized<5981>(perm, a);
    case 782:
        return ScrambledRadicalInverseSpecialized<5987>(perm, a);
    case 783:
        return ScrambledRadicalInverseSpecialized<6007>(perm, a);
    case 784:
        return ScrambledRadicalInverseSpecialized<6011>(perm, a);
    case 785:
        return ScrambledRadicalInverseSpecialized<6029>(perm, a);
    case 786:
        return ScrambledRadicalInverseSpecialized<6037>(perm, a);
    case 787:
        return ScrambledRadicalInverseSpecialized<6043>(perm, a);
    case 788:
        return ScrambledRadicalInverseSpecialized<6047>(perm, a);
    case 789:
        return ScrambledRadicalInverseSpecialized<6053>(perm, a);
    case 790:
        return ScrambledRadicalInverseSpecialized<6067>(perm, a);
    case 791:
        return ScrambledRadicalInverseSpecialized<6073>(perm, a);
    case 792:
        return ScrambledRadicalInverseSpecialized<6079>(perm, a);
    case 793:
        return ScrambledRadicalInverseSpecialized<6089>(perm, a);
    case 794:
        return ScrambledRadicalInverseSpecialized<6091>(perm, a);
    case 795:
        return ScrambledRadicalInverseSpecialized<6101>(perm, a);
    case 796:
        return ScrambledRadicalInverseSpecialized<6113>(perm, a);
    case 797:
        return ScrambledRadicalInverseSpecialized<6121>(perm, a);
    case 798:
        return ScrambledRadicalInverseSpecialized<6131>(perm, a);
    case 799:
        return ScrambledRadicalInverseSpecialized<6133>(perm, a);
    case 800:
        return ScrambledRadicalInverseSpecialized<6143>(perm, a);
    case 801:
        return ScrambledRadicalInverseSpecialized<6151>(perm, a);
    case 802:
        return ScrambledRadicalInverseSpecialized<6163>(perm, a);
    case 803:
        return ScrambledRadicalInverseSpecialized<6173>(perm, a);
    case 804:
        return ScrambledRadicalInverseSpecialized<6197>(perm, a);
    case 805:
        return ScrambledRadicalInverseSpecialized<6199>(perm, a);
    case 806:
        return ScrambledRadicalInverseSpecialized<6203>(perm, a);
    case 807:
        return ScrambledRadicalInverseSpecialized<6211>(perm, a);
    case 808:
        return ScrambledRadicalInverseSpecialized<6217>(perm, a);
    case 809:
        return ScrambledRadicalInverseSpecialized<6221>(perm, a);
    case 810:
        return ScrambledRadicalInverseSpecialized<6229>(perm, a);
    case 811:
        return ScrambledRadicalInverseSpecialized<6247>(perm, a);
    case 812:
        return ScrambledRadicalInverseSpecialized<6257>(perm, a);
    case 813:
        return ScrambledRadicalInverseSpecialized<6263>(perm, a);
    case 814:
        return ScrambledRadicalInverseSpecialized<6269>(perm, a);
    case 815:
        return ScrambledRadicalInverseSpecialized<6271>(perm, a);
    case 816:
        return ScrambledRadicalInverseSpecialized<6277>(perm, a);
    case 817:
        return ScrambledRadicalInverseSpecialized<6287>(perm, a);
    case 818:
        return ScrambledRadicalInverseSpecialized<6299>(perm, a);
    case 819:
        return ScrambledRadicalInverseSpecialized<6301>(perm, a);
    case 820:
        return ScrambledRadicalInverseSpecialized<6311>(perm, a);
    case 821:
        return ScrambledRadicalInverseSpecialized<6317>(perm, a);
    case 822:
        return ScrambledRadicalInverseSpecialized<6323>(perm, a);
    case 823:
        return ScrambledRadicalInverseSpecialized<6329>(perm, a);
    case 824:
        return ScrambledRadicalInverseSpecialized<6337>(perm, a);
    case 825:
        return ScrambledRadicalInverseSpecialized<6343>(perm, a);
    case 826:
        return ScrambledRadicalInverseSpecialized<6353>(perm, a);
    case 827:
        return ScrambledRadicalInverseSpecialized<6359>(perm, a);
    case 828:
        return ScrambledRadicalInverseSpecialized<6361>(perm, a);
    case 829:
        return ScrambledRadicalInverseSpecialized<6367>(perm, a);
    case 830:
        return ScrambledRadicalInverseSpecialized<6373>(perm, a);
    case 831:
        return ScrambledRadicalInverseSpecialized<6379>(perm, a);
    case 832:
        return ScrambledRadicalInverseSpecialized<6389>(perm, a);
    case 833:
        return ScrambledRadicalInverseSpecialized<6397>(perm, a);
    case 834:
        return ScrambledRadicalInverseSpecialized<6421>(perm, a);
    case 835:
        return ScrambledRadicalInverseSpecialized<6427>(perm, a);
    case 836:
        return ScrambledRadicalInverseSpecialized<6449>(perm, a);
    case 837:
        return ScrambledRadicalInverseSpecialized<6451>(perm, a);
    case 838:
        return ScrambledRadicalInverseSpecialized<6469>(perm, a);
    case 839:
        return ScrambledRadicalInverseSpecialized<6473>(perm, a);
    case 840:
        return ScrambledRadicalInverseSpecialized<6481>(perm, a);
    case 841:
        return ScrambledRadicalInverseSpecialized<6491>(perm, a);
    case 842:
        return ScrambledRadicalInverseSpecialized<6521>(perm, a);
    case 843:
        return ScrambledRadicalInverseSpecialized<6529>(perm, a);
    case 844:
        return ScrambledRadicalInverseSpecialized<6547>(perm, a);
    case 845:
        return ScrambledRadicalInverseSpecialized<6551>(perm, a);
    case 846:
        return ScrambledRadicalInverseSpecialized<6553>(perm, a);
    case 847:
        return ScrambledRadicalInverseSpecialized<6563>(perm, a);
    case 848:
        return ScrambledRadicalInverseSpecialized<6569>(perm, a);
    case 849:
        return ScrambledRadicalInverseSpecialized<6571>(perm, a);
    case 850:
        return ScrambledRadicalInverseSpecialized<6577>(perm, a);
    case 851:
        return ScrambledRadicalInverseSpecialized<6581>(perm, a);
    case 852:
        return ScrambledRadicalInverseSpecialized<6599>(perm, a);
    case 853:
        return ScrambledRadicalInverseSpecialized<6607>(perm, a);
    case 854:
        return ScrambledRadicalInverseSpecialized<6619>(perm, a);
    case 855:
        return ScrambledRadicalInverseSpecialized<6637>(perm, a);
    case 856:
        return ScrambledRadicalInverseSpecialized<6653>(perm, a);
    case 857:
        return ScrambledRadicalInverseSpecialized<6659>(perm, a);
    case 858:
        return ScrambledRadicalInverseSpecialized<6661>(perm, a);
    case 859:
        return ScrambledRadicalInverseSpecialized<6673>(perm, a);
    case 860:
        return ScrambledRadicalInverseSpecialized<6679>(perm, a);
    case 861:
        return ScrambledRadicalInverseSpecialized<6689>(perm, a);
    case 862:
        return ScrambledRadicalInverseSpecialized<6691>(perm, a);
    case 863:
        return ScrambledRadicalInverseSpecialized<6701>(perm, a);
    case 864:
        return ScrambledRadicalInverseSpecialized<6703>(perm, a);
    case 865:
        return ScrambledRadicalInverseSpecialized<6709>(perm, a);
    case 866:
        return ScrambledRadicalInverseSpecialized<6719>(perm, a);
    case 867:
        return ScrambledRadicalInverseSpecialized<6733>(perm, a);
    case 868:
        return ScrambledRadicalInverseSpecialized<6737>(perm, a);
    case 869:
        return ScrambledRadicalInverseSpecialized<6761>(perm, a);
    case 870:
        return ScrambledRadicalInverseSpecialized<6763>(perm, a);
    case 871:
        return ScrambledRadicalInverseSpecialized<6779>(perm, a);
    case 872:
        return ScrambledRadicalInverseSpecialized<6781>(perm, a);
    case 873:
        return ScrambledRadicalInverseSpecialized<6791>(perm, a);
    case 874:
        return ScrambledRadicalInverseSpecialized<6793>(perm, a);
    case 875:
        return ScrambledRadicalInverseSpecialized<6803>(perm, a);
    case 876:
        return ScrambledRadicalInverseSpecialized<6823>(perm, a);
    case 877:
        return ScrambledRadicalInverseSpecialized<6827>(perm, a);
    case 878:
        return ScrambledRadicalInverseSpecialized<6829>(perm, a);
    case 879:
        return ScrambledRadicalInverseSpecialized<6833>(perm, a);
    case 880:
        return ScrambledRadicalInverseSpecialized<6841>(perm, a);
    case 881:
        return ScrambledRadicalInverseSpecialized<6857>(perm, a);
    case 882:
        return ScrambledRadicalInverseSpecialized<6863>(perm, a);
    case 883:
        return ScrambledRadicalInverseSpecialized<6869>(perm, a);
    case 884:
        return ScrambledRadicalInverseSpecialized<6871>(perm, a);
    case 885:
        return ScrambledRadicalInverseSpecialized<6883>(perm, a);
    case 886:
        return ScrambledRadicalInverseSpecialized<6899>(perm, a);
    case 887:
        return ScrambledRadicalInverseSpecialized<6907>(perm, a);
    case 888:
        return ScrambledRadicalInverseSpecialized<6911>(perm, a);
    case 889:
        return ScrambledRadicalInverseSpecialized<6917>(perm, a);
    case 890:
        return ScrambledRadicalInverseSpecialized<6947>(perm, a);
    case 891:
        return ScrambledRadicalInverseSpecialized<6949>(perm, a);
    case 892:
        return ScrambledRadicalInverseSpecialized<6959>(perm, a);
    case 893:
        return ScrambledRadicalInverseSpecialized<6961>(perm, a);
    case 894:
        return ScrambledRadicalInverseSpecialized<6967>(perm, a);
    case 895:
        return ScrambledRadicalInverseSpecialized<6971>(perm, a);
    case 896:
        return ScrambledRadicalInverseSpecialized<6977>(perm, a);
    case 897:
        return ScrambledRadicalInverseSpecialized<6983>(perm, a);
    case 898:
        return ScrambledRadicalInverseSpecialized<6991>(perm, a);
    case 899:
        return ScrambledRadicalInverseSpecialized<6997>(perm, a);
    case 900:
        return ScrambledRadicalInverseSpecialized<7001>(perm, a);
    case 901:
        return ScrambledRadicalInverseSpecialized<7013>(perm, a);
    case 902:
        return ScrambledRadicalInverseSpecialized<7019>(perm, a);
    case 903:
        return ScrambledRadicalInverseSpecialized<7027>(perm, a);
    case 904:
        return ScrambledRadicalInverseSpecialized<7039>(perm, a);
    case 905:
        return ScrambledRadicalInverseSpecialized<7043>(perm, a);
    case 906:
        return ScrambledRadicalInverseSpecialized<7057>(perm, a);
    case 907:
        return ScrambledRadicalInverseSpecialized<7069>(perm, a);
    case 908:
        return ScrambledRadicalInverseSpecialized<7079>(perm, a);
    case 909:
        return ScrambledRadicalInverseSpecialized<7103>(perm, a);
    case 910:
        return ScrambledRadicalInverseSpecialized<7109>(perm, a);
    case 911:
        return ScrambledRadicalInverseSpecialized<7121>(perm, a);
    case 912:
        return ScrambledRadicalInverseSpecialized<7127>(perm, a);
    case 913:
        return ScrambledRadicalInverseSpecialized<7129>(perm, a);
    case 914:
        return ScrambledRadicalInverseSpecialized<7151>(perm, a);
    case 915:
        return ScrambledRadicalInverseSpecialized<7159>(perm, a);
    case 916:
        return ScrambledRadicalInverseSpecialized<7177>(perm, a);
    case 917:
        return ScrambledRadicalInverseSpecialized<7187>(perm, a);
    case 918:
        return ScrambledRadicalInverseSpecialized<7193>(perm, a);
    case 919:
        return ScrambledRadicalInverseSpecialized<7207>(perm, a);
    case 920:
        return ScrambledRadicalInverseSpecialized<7211>(perm, a);
    case 921:
        return ScrambledRadicalInverseSpecialized<7213>(perm, a);
    case 922:
        return ScrambledRadicalInverseSpecialized<7219>(perm, a);
    case 923:
        return ScrambledRadicalInverseSpecialized<7229>(perm, a);
    case 924:
        return ScrambledRadicalInverseSpecialized<7237>(perm, a);
    case 925:
        return ScrambledRadicalInverseSpecialized<7243>(perm, a);
    case 926:
        return ScrambledRadicalInverseSpecialized<7247>(perm, a);
    case 927:
        return ScrambledRadicalInverseSpecialized<7253>(perm, a);
    case 928:
        return ScrambledRadicalInverseSpecialized<7283>(perm, a);
    case 929:
        return ScrambledRadicalInverseSpecialized<7297>(perm, a);
    case 930:
        return ScrambledRadicalInverseSpecialized<7307>(perm, a);
    case 931:
        return ScrambledRadicalInverseSpecialized<7309>(perm, a);
    case 932:
        return ScrambledRadicalInverseSpecialized<7321>(perm, a);
    case 933:
        return ScrambledRadicalInverseSpecialized<7331>(perm, a);
    case 934:
        return ScrambledRadicalInverseSpecialized<7333>(perm, a);
    case 935:
        return ScrambledRadicalInverseSpecialized<7349>(perm, a);
    case 936:
        return ScrambledRadicalInverseSpecialized<7351>(perm, a);
    case 937:
        return ScrambledRadicalInverseSpecialized<7369>(perm, a);
    case 938:
        return ScrambledRadicalInverseSpecialized<7393>(perm, a);
    case 939:
        return ScrambledRadicalInverseSpecialized<7411>(perm, a);
    case 940:
        return ScrambledRadicalInverseSpecialized<7417>(perm, a);
    case 941:
        return ScrambledRadicalInverseSpecialized<7433>(perm, a);
    case 942:
        return ScrambledRadicalInverseSpecialized<7451>(perm, a);
    case 943:
        return ScrambledRadicalInverseSpecialized<7457>(perm, a);
    case 944:
        return ScrambledRadicalInverseSpecialized<7459>(perm, a);
    case 945:
        return ScrambledRadicalInverseSpecialized<7477>(perm, a);
    case 946:
        return ScrambledRadicalInverseSpecialized<7481>(perm, a);
    case 947:
        return ScrambledRadicalInverseSpecialized<7487>(perm, a);
    case 948:
        return ScrambledRadicalInverseSpecialized<7489>(perm, a);
    case 949:
        return ScrambledRadicalInverseSpecialized<7499>(perm, a);
    case 950:
        return ScrambledRadicalInverseSpecialized<7507>(perm, a);
    case 951:
        return ScrambledRadicalInverseSpecialized<7517>(perm, a);
    case 952:
        return ScrambledRadicalInverseSpecialized<7523>(perm, a);
    case 953:
        return ScrambledRadicalInverseSpecialized<7529>(perm, a);
    case 954:
        return ScrambledRadicalInverseSpecialized<7537>(perm, a);
    case 955:
        return ScrambledRadicalInverseSpecialized<7541>(perm, a);
    case 956:
        return ScrambledRadicalInverseSpecialized<7547>(perm, a);
    case 957:
        return ScrambledRadicalInverseSpecialized<7549>(perm, a);
    case 958:
        return ScrambledRadicalInverseSpecialized<7559>(perm, a);
    case 959:
        return ScrambledRadicalInverseSpecialized<7561>(perm, a);
    case 960:
        return ScrambledRadicalInverseSpecialized<7573>(perm, a);
    case 961:
        return ScrambledRadicalInverseSpecialized<7577>(perm, a);
    case 962:
        return ScrambledRadicalInverseSpecialized<7583>(perm, a);
    case 963:
        return ScrambledRadicalInverseSpecialized<7589>(perm, a);
    case 964:
        return ScrambledRadicalInverseSpecialized<7591>(perm, a);
    case 965:
        return ScrambledRadicalInverseSpecialized<7603>(perm, a);
    case 966:
        return ScrambledRadicalInverseSpecialized<7607>(perm, a);
    case 967:
        return ScrambledRadicalInverseSpecialized<7621>(perm, a);
    case 968:
        return ScrambledRadicalInverseSpecialized<7639>(perm, a);
    case 969:
        return ScrambledRadicalInverseSpecialized<7643>(perm, a);
    case 970:
        return ScrambledRadicalInverseSpecialized<7649>(perm, a);
    case 971:
        return ScrambledRadicalInverseSpecialized<7669>(perm, a);
    case 972:
        return ScrambledRadicalInverseSpecialized<7673>(perm, a);
    case 973:
        return ScrambledRadicalInverseSpecialized<7681>(perm, a);
    case 974:
        return ScrambledRadicalInverseSpecialized<7687>(perm, a);
    case 975:
        return ScrambledRadicalInverseSpecialized<7691>(perm, a);
    case 976:
        return ScrambledRadicalInverseSpecialized<7699>(perm, a);
    case 977:
        return ScrambledRadicalInverseSpecialized<7703>(perm, a);
    case 978:
        return ScrambledRadicalInverseSpecialized<7717>(perm, a);
    case 979:
        return ScrambledRadicalInverseSpecialized<7723>(perm, a);
    case 980:
        return ScrambledRadicalInverseSpecialized<7727>(perm, a);
    case 981:
        return ScrambledRadicalInverseSpecialized<7741>(perm, a);
    case 982:
        return ScrambledRadicalInverseSpecialized<7753>(perm, a);
    case 983:
        return ScrambledRadicalInverseSpecialized<7757>(perm, a);
    case 984:
        return ScrambledRadicalInverseSpecialized<7759>(perm, a);
    case 985:
        return ScrambledRadicalInverseSpecialized<7789>(perm, a);
    case 986:
        return ScrambledRadicalInverseSpecialized<7793>(perm, a);
    case 987:
        return ScrambledRadicalInverseSpecialized<7817>(perm, a);
    case 988:
        return ScrambledRadicalInverseSpecialized<7823>(perm, a);
    case 989:
        return ScrambledRadicalInverseSpecialized<7829>(perm, a);
    case 990:
        return ScrambledRadicalInverseSpecialized<7841>(perm, a);
    case 991:
        return ScrambledRadicalInverseSpecialized<7853>(perm, a);
    case 992:
        return ScrambledRadicalInverseSpecialized<7867>(perm, a);
    case 993:
        return ScrambledRadicalInverseSpecialized<7873>(perm, a);
    case 994:
        return ScrambledRadicalInverseSpecialized<7877>(perm, a);
    case 995:
        return ScrambledRadicalInverseSpecialized<7879>(perm, a);
    case 996:
        return ScrambledRadicalInverseSpecialized<7883>(perm, a);
    case 997:
        return ScrambledRadicalInverseSpecialized<7901>(perm, a);
    case 998:
        return ScrambledRadicalInverseSpecialized<7907>(perm, a);
    case 999:
        return ScrambledRadicalInverseSpecialized<7919>(perm, a);
    case 1000:
        return ScrambledRadicalInverseSpecialized<7927>(perm, a);
    case 1001:
        return ScrambledRadicalInverseSpecialized<7933>(perm, a);
    case 1002:
        return ScrambledRadicalInverseSpecialized<7937>(perm, a);
    case 1003:
        return ScrambledRadicalInverseSpecialized<7949>(perm, a);
    case 1004:
        return ScrambledRadicalInverseSpecialized<7951>(perm, a);
    case 1005:
        return ScrambledRadicalInverseSpecialized<7963>(perm, a);
    case 1006:
        return ScrambledRadicalInverseSpecialized<7993>(perm, a);
    case 1007:
        return ScrambledRadicalInverseSpecialized<8009>(perm, a);
    case 1008:
        return ScrambledRadicalInverseSpecialized<8011>(perm, a);
    case 1009:
        return ScrambledRadicalInverseSpecialized<8017>(perm, a);
    case 1010:
        return ScrambledRadicalInverseSpecialized<8039>(perm, a);
    case 1011:
        return ScrambledRadicalInverseSpecialized<8053>(perm, a);
    case 1012:
        return ScrambledRadicalInverseSpecialized<8059>(perm, a);
    case 1013:
        return ScrambledRadicalInverseSpecialized<8069>(perm, a);
    case 1014:
        return ScrambledRadicalInverseSpecialized<8081>(perm, a);
    case 1015:
        return ScrambledRadicalInverseSpecialized<8087>(perm, a);
    case 1016:
        return ScrambledRadicalInverseSpecialized<8089>(perm, a);
    case 1017:
        return ScrambledRadicalInverseSpecialized<8093>(perm, a);
    case 1018:
        return ScrambledRadicalInverseSpecialized<8101>(perm, a);
    case 1019:
        return ScrambledRadicalInverseSpecialized<8111>(perm, a);
    case 1020:
        return ScrambledRadicalInverseSpecialized<8117>(perm, a);
    case 1021:
        return ScrambledRadicalInverseSpecialized<8123>(perm, a);
    case 1022:
        return ScrambledRadicalInverseSpecialized<8147>(perm, a);
    case 1023:
        return ScrambledRadicalInverseSpecialized<8161>(perm, a);
    default:
        logger(EError, "Base %d is >= 1024, the limit of ScrambledRadicalInverse", baseIndex);
        return 0;
    }
}

} // namespace lightwave