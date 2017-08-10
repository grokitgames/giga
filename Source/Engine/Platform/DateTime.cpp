
#include <giga-engine.h>

std::string DateTime::ToString() {
    // Prepare output string
    std::string output;
    output.resize(100);
    
    // Convert timestamp to local time
    tm* local = localtime(&m_time);
    sprintf((char*)output.data(), "%d-%02d-%02d %02d:%02d:%02d",
            local->tm_year + 1900,
            local->tm_mon,
            local->tm_mday,
            local->tm_hour,
            local->tm_min,
            local->tm_sec);
    
    return(output);
}

DateTime* DateTime::GetCurrent() {
    DateTime* dt = new DateTime();
    time(&dt->m_time);
    return(dt);
}

uint64_t DateTime::GetTimestamp() {
    return(m_time);
}

int DateTime::GetYear() {
    tm* local = localtime(&m_time);
    return(local->tm_year + 1900);
}

int DateTime::GetMonth() {
    tm* local = localtime(&m_time);
    return(local->tm_mon);
}

int DateTime::GetDay() {
    tm* local = localtime(&m_time);
    return(local->tm_mday);
}

int DateTime::GetHour() {
    tm* local = localtime(&m_time);
    return(local->tm_hour);
}

int DateTime::GetMinute() {
    tm* local = localtime(&m_time);
    return(local->tm_min);
}

int DateTime::GetSecond() {
    tm* local = localtime(&m_time);
    return(local->tm_sec);
}

Variant* DateTime::GetCurrent(Variant* object, int argc, Variant** argv) {
    GIGA_ASSERT(argc == 0, "GetCurrent expects no parameters");
    return(new Variant(DateTime::GetCurrent()));
}

Variant* DateTime::GetTimestamp(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    return(new Variant((int64_t)dt->GetTimestamp()));
}

Variant* DateTime::GetYear(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_year + 1900));
}

Variant* DateTime::GetMonth(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_mon));
}

Variant* DateTime::GetDay(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_mday));
}

Variant* DateTime::GetHour(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_hour));
}

Variant* DateTime::GetMinute(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_min));
}

Variant* DateTime::GetSecond(Variant* object) {
    DateTime* dt = object->AsObject<DateTime>();
    tm* local = localtime(&dt->m_time);
    return(new Variant(local->tm_sec));
}
