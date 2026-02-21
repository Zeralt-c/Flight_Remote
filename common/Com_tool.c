#include "Com_tool.h"

/**限幅函数
 * 对输入值进行限幅处理，使其保持在指定范围内
 * @param value 输入值
 * @param min 最小值
 * @param max 最大值
 * @return 限幅后的值
 */

int16_t Com_limit(int16_t value, int16_t min, int16_t max) {
    if (value < min) {
        return min;
    } else if (value > max) {
        return max;
    } else {
        return value;
    }
}
