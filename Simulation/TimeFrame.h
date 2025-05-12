#pragma once


enum class TimeFrame {
    MINUTE,
    FIVEMINTUES,
    HOUR,
    DAY
};

inline double getTicksPerDay(TimeFrame tf) {
    switch (tf) {
        case TimeFrame::MINUTE: return 390.0;
        case TimeFrame::FIVEMINTUES: return 78.0;
        case TimeFrame::HOUR: return 6.5;
        case TimeFrame::DAY: return 1.0;
        default: return 1.0;
    }
}