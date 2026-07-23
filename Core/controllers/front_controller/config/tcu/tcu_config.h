#ifndef CONFIG_TCU_CONFIG_H_
#define CONFIG_TCU_CONFIG_H_

/*============================================================================*/
/* APPS Configuration - Sensor 1                                              */
/*============================================================================*/

#define TCU_APPS_1_VOLTAGE_MIN_V             2.0f
#define TCU_APPS_1_VOLTAGE_MAX_V             11.5f

#define TCU_APPS_1_ACTIVE_MIN_V              4.9f
#define TCU_APPS_1_ACTIVE_MAX_V              9.95f

#define TCU_APPS_1_SCALING_FACTOR            4.85f

#define TCU_APPS_1_POSITIVE_SLOPE            true

/*============================================================================*/
/* APPS Configuration - Sensor 2                                              */
/*============================================================================*/

#define TCU_APPS_2_VOLTAGE_MIN_V             2.0f
#define TCU_APPS_2_VOLTAGE_MAX_V             11.8f

#define TCU_APPS_2_ACTIVE_MIN_V              8.15f
#define TCU_APPS_2_ACTIVE_MAX_V              11.2f

#define TCU_APPS_2_SCALING_FACTOR            4.85f

#define TCU_APPS_2_POSITIVE_SLOPE            true

/*============================================================================*/
/* APPS Plausibility Configuration                                            */
/*============================================================================*/

#define TCU_APPS_IMPLAUSIBILITY_THRESHOLD_PCT   20.0f

#define TCU_APPS_IMPLAUSIBILITY_TIMEOUT_MS      1000U

/*============================================================================*/
/* BSE Configuration                                                          */
/*============================================================================*/

#define TCU_BSE_1_VOLTAGE_MIN_V               0.5f
#define TCU_BSE_1_VOLTAGE_MAX_V               4.5f

#define TCU_BSE_1_ACTIVE_MIN_V                0.83f
#define TCU_BSE_1_ACTIVE_MAX_V                4.5f

#define TCU_BSE_1_SCALING_FACTOR              6.73f

#define TCU_BSE_1_PSI_MAX                     700.0f

#define TCU_BSE_1_BRAKING_THRESHOLD_PCT       10.0f

#define TCU_BSE_1_HARD_BRAKING_THRESHOLD_PCT  20.0f

#define TCU_BSE_1_IMPLAUSIBILITY_TIMEOUT_MS   100U

/*============================================================================*/
/* APPS/BSE Plausibility Configuration                                        */
/*============================================================================*/

#define TCU_APPS_BSE_PLAUSIBILITY_APPS_THRESHOLD_PCT    25.0f

#define TCU_APPS_BSE_PLAUSIBILITY_RELEASE_PCT           5.0f


#endif /* CONFIG_TCU_CONFIG_H_ */