# Tests
## Rays
add_executable(Rays ${CMAKE_CURRENT_LIST_DIR}/rays.cpp)
target_link_libraries(Rays PRIVATE CHESS)

add_test(bitshift_rays_a1_north Rays "a1" "8" 72340172838076672)
add_test(bitshift_rays_a1_east Rays "a1" "1" 254)
add_test(bitshift_rays_a1_south Rays "a1" "-8" 0)
add_test(bitshift_rays_a1_west Rays "a1" "-1" 0)

add_test(bitshift_rays_h1_north Rays "h1" "8" 9259542123273814016)
add_test(bitshift_rays_h1_east Rays "h1" "1" 0)
add_test(bitshift_rays_h1_south Rays "h1" "-8" 0)
add_test(bitshift_rays_h1_west Rays "h1" "-1" 127)

add_test(bitshift_rays_a8_north Rays "a8" "8" 0)
add_test(bitshift_rays_a8_east Rays "a8" "1" 18302628885633695744)
add_test(bitshift_rays_a8_south Rays "a8" "-8" 282578800148737)
add_test(bitshift_rays_a8_west Rays "a8" "-1" 0)

add_test(bitshift_rays_h8_north Rays "h8" "8" 0)
add_test(bitshift_rays_h8_east Rays "h8" "1" 0)
add_test(bitshift_rays_h8_south Rays "h8" "-8" 36170086419038336)
add_test(bitshift_rays_h8_west Rays "h8" "-1" 9151314442816847872)

## SqRays
add_executable(SqRays ${CMAKE_CURRENT_LIST_DIR}/sq_rays.cpp)
target_link_libraries(SqRays PRIVATE CHESS)

add_test(bitshift_sqrays_a1_a2 SqRays "a1" "a2" 72340172838076672)
add_test(bitshift_sqrays_a1_b2 SqRays "a1" "b2" 9241421688590303744)
add_test(bitshift_sqrays_a1_b1 SqRays "a1" "b1" 254)

add_test(bitshift_sqrays_h1_h2 SqRays "h1" "h2" 9259542123273814016)
add_test(bitshift_sqrays_h1_b2 SqRays "h1" "g2" 72624976668147712)
add_test(bitshift_sqrays_h1_b1 SqRays "h1" "g1" 127)

add_test(bitshift_sqrays_a8_a7 SqRays "a8" "a7" 282578800148737)
add_test(bitshift_sqrays_a8_b7 SqRays "a8" "b7" 567382630219904)
add_test(bitshift_sqrays_a8_b8 SqRays "a8" "b8" 18302628885633695744)

add_test(bitshift_sqrays_h8_h7 SqRays "h8" "h7" 36170086419038336)
add_test(bitshift_sqrays_h8_g7 SqRays "h8" "g7" 18049651735527937)
add_test(bitshift_sqrays_h8_g8 SqRays "h8" "g8" 9151314442816847872)

## XRays
add_executable(XRays ${CMAKE_CURRENT_LIST_DIR}/xrays.cpp)
target_link_libraries(XRays PRIVATE CHESS)

add_test(bitshift_xrays_a1_a2 XRays "a1" "a2" 72340172838076416)
add_test(bitshift_xrays_a1_b2 XRays "a1" "b2" 9241421688590303232)
add_test(bitshift_xrays_a1_b1 XRays "a1" "b1" 252)

add_test(bitshift_xrays_h1_h2 XRays "h1" "h2" 9259542123273781248)
add_test(bitshift_xrays_h1_b2 XRays "h1" "g2" 72624976668131328)
add_test(bitshift_xrays_h1_b1 XRays "h1" "g1" 63)

add_test(bitshift_xrays_a8_a7 XRays "a8" "a7" 1103823438081)
add_test(bitshift_xrays_a8_b7 XRays "a8" "b7" 4432676798592)
add_test(bitshift_xrays_a8_b8 XRays "a8" "b8" 18158513697557839872)

add_test(bitshift_xrays_h8_h7 XRays "h8" "h7" 141289400074368)
add_test(bitshift_xrays_h8_g7 XRays "h8" "g7" 35253226045953)
add_test(bitshift_xrays_h8_g8 XRays "h8" "g8" 4539628424389459968)

## Rings
add_executable(Rings ${CMAKE_CURRENT_LIST_DIR}/rings.cpp)
target_link_libraries(Rings PRIVATE CHESS)

add_test(bitshift_rings_a1_1 Rings "a1" "1" 770)
add_test(bitshift_rings_a1_2 Rings "a1" "2" 459780)
add_test(bitshift_rings_a1_3 Rings "a1" "3" 252184584)
add_test(bitshift_rings_a1_4 Rings "a1" "4" 133413474320)
add_test(bitshift_rings_a1_5 Rings "a1" "5" 69407210479648)
add_test(bitshift_rings_a1_6 Rings "a1" "6" 35817966742290496)
add_test(bitshift_rings_a1_7 Rings "a1" "7" 18410856566090662016)

add_test(bitshift_rings_h1_1 Rings "h1" "1" 49216)
add_test(bitshift_rings_h1_2 Rings "h1" "2" 14688288)
add_test(bitshift_rings_h1_3 Rings "h1" "3" 4027584528)
add_test(bitshift_rings_h1_4 Rings "h1" "4" 1065286633480)
add_test(bitshift_rings_h1_5 Rings "h1" "5" 277094177440772)
add_test(bitshift_rings_h1_6 Rings "h1" "6" 71496851731382786)
add_test(bitshift_rings_h1_7 Rings "h1" "7" 18374969058471772417)

add_test(bitshift_rings_a8_1 Rings "a8" "1" 144959613005987840)
add_test(bitshift_rings_a8_2 Rings "a8" "2" 289363972639948800)
add_test(bitshift_rings_a8_3 Rings "a8" "3" 578721412634640384)
add_test(bitshift_rings_a8_4 Rings "a8" "4" 1157442765659832320)
add_test(bitshift_rings_a8_5 Rings "a8" "5" 2314885530820476928)
add_test(bitshift_rings_a8_6 Rings "a8" "6" 4629771061636923136)
add_test(bitshift_rings_a8_7 Rings "a8" "7" 9259542123273814271)

add_test(bitshift_rings_h8_1 Rings "h8" "1" 4665729213955833856)
add_test(bitshift_rings_h8_2 Rings "h8" "2" 2315096499073056768)
add_test(bitshift_rings_h8_3 Rings "h8" "3" 1157443727212412928)
add_test(bitshift_rings_h8_4 Rings "h8" "4" 578721386730618880)
add_test(bitshift_rings_h8_5 Rings "h8" "5" 289360691368558592)
add_test(bitshift_rings_h8_6 Rings "h8" "6" 144680345676217856)
add_test(bitshift_rings_h8_7 Rings "h8" "7" 72340172838076927)

## Moves
add_executable(Moves ${CMAKE_CURRENT_LIST_DIR}/moves.cpp)
target_link_libraries(Moves PRIVATE CHESS)

add_test(bitshift_moves_b_a1 Moves "b" "a1" 9241421688590303744)
add_test(bitshift_moves_b_h1 Moves "b" "h1" 72624976668147712)
add_test(bitshift_moves_b_a8 Moves "b" "a8" 567382630219904)
add_test(bitshift_moves_b_h8 Moves "b" "h8" 18049651735527937)
add_test(bitshift_moves_b_d4 Moves "b" "d4" 9241705379636978241)
add_test(bitshift_moves_b_d5 Moves "b" "d5" 4693335752243822976)
add_test(bitshift_moves_b_e4 Moves "b" "e4" 108724279602332802)
add_test(bitshift_moves_b_e5 Moves "b" "e5" 9386671504487645697)

add_test(bitshift_moves_n_a1 Moves "n" "a1" 132096)
add_test(bitshift_moves_n_h1 Moves "n" "h1" 4202496)
add_test(bitshift_moves_n_a8 Moves "n" "a8" 1128098930098176)
add_test(bitshift_moves_n_h8 Moves "n" "h8" 9077567998918656)
add_test(bitshift_moves_n_d4 Moves "n" "d4" 22136263676928)
add_test(bitshift_moves_n_d5 Moves "n" "d5" 5666883501293568)
add_test(bitshift_moves_n_e4 Moves "n" "e4" 44272527353856)
add_test(bitshift_moves_n_e5 Moves "n" "e5" 11333767002587136)

add_test(bitshift_moves_r_a1 Moves "r" "a1" 72340172838076926)
add_test(bitshift_moves_r_h1 Moves "r" "h1" 9259542123273814143)
add_test(bitshift_moves_r_a8 Moves "r" "a8" 18302911464433844481)
add_test(bitshift_moves_r_h8 Moves "r" "h8" 9187484529235886208)
add_test(bitshift_moves_r_d4 Moves "r" "d4" 578721386714368008)
add_test(bitshift_moves_r_d5 Moves "r" "d5" 578722409201797128)
add_test(bitshift_moves_r_e4 Moves "r" "e4" 1157442769150545936)
add_test(bitshift_moves_r_e5 Moves "r" "e5" 1157443723186933776)

add_test(bitshift_moves_q_a1 Moves "q" "a1" 9313761861428380670)
add_test(bitshift_moves_q_h1 Moves "q" "h1" 9332167099941961855)
add_test(bitshift_moves_q_a8 Moves "q" "a8" 18303478847064064385)
add_test(bitshift_moves_q_h8 Moves "q" "h8" 9205534180971414145)
add_test(bitshift_moves_q_d4 Moves "q" "d4" 9820426766351346249)
add_test(bitshift_moves_q_d5 Moves "q" "d5" 5272058161445620104)
add_test(bitshift_moves_q_e4 Moves "q" "e4" 1266167048752878738)
add_test(bitshift_moves_q_e5 Moves "q" "e5" 10544115227674579473)

add_test(bitshift_moves_k_a1 Moves "k" "a1" 770)
add_test(bitshift_moves_k_h1 Moves "k" "h1" 49216)
add_test(bitshift_moves_k_a8 Moves "k" "a8" 144959613005987840)
add_test(bitshift_moves_k_h8 Moves "k" "h8" 4665729213955833856)
add_test(bitshift_moves_k_d4 Moves "k" "d4" 120596463616)
add_test(bitshift_moves_k_d5 Moves "k" "d5" 30872694685696)
add_test(bitshift_moves_k_e4 Moves "k" "e4" 241192927232)
add_test(bitshift_moves_k_e4 Moves "k" "e5" 61745389371392)

## BB
add_executable(bb ${CMAKE_CURRENT_LIST_DIR}/bb.cpp)
target_link_libraries(bb PRIVATE CHESS)

add_test(bitshift_bb_b_a1 bb "b" "a1" 18049651735527936)
add_test(bitshift_bb_b_h1 bb "b" "h1" 567382630219776)
add_test(bitshift_bb_b_a8 bb "b" "a8" 567382630219776)
add_test(bitshift_bb_b_h8 bb "b" "h8" 18049651735527936)
add_test(bitshift_bb_b_d4 bb "b" "d4" 18051867805491712)
add_test(bitshift_bb_b_d5 bb "b" "d5" 9592139778506752)
add_test(bitshift_bb_b_e4 bb "b" "e4" 637888545440768)
add_test(bitshift_bb_b_e5 bb "b" "e5" 19184279556981248)

add_test(bitshift_bb_r_a1 bb "r" "a1" 282578800148862)
add_test(bitshift_bb_r_h1 bb "r" "h1" 36170086419038334)
add_test(bitshift_bb_r_a8 bb "r" "a8" 9079539427579068672)
add_test(bitshift_bb_r_h8 bb "r" "h8" 9115426935197958144)
add_test(bitshift_bb_r_d4 bb "r" "d4" 2260632246683648)
add_test(bitshift_bb_r_d5 bb "r" "d5" 2261102847592448)
add_test(bitshift_bb_r_e4 bb "r" "e4" 4521262379438080)
add_test(bitshift_bb_r_e5 bb "r" "e5" 4521664529305600)

add_test(bitshift_bb_q_a1 bb "q" "a1" 18332230535676798)
add_test(bitshift_bb_q_h1 bb "q" "h1" 36737469049258110)
add_test(bitshift_bb_q_a8 bb "q" "a8" 9080106810209288448)
add_test(bitshift_bb_q_h8 bb "q" "h8" 9133476586933486080)
add_test(bitshift_bb_q_d4 bb "q" "d4" 20312500052175360)
add_test(bitshift_bb_q_d5 bb "q" "d5" 11853242626099200)
add_test(bitshift_bb_q_e4 bb "q" "e4" 5159150924878848)
add_test(bitshift_bb_q_e5 bb "q" "e5" 23705944086286848)
