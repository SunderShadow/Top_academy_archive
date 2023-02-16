#include <iostream>
#include <time.h>

using namespace std;

tm humanBirthDate(const unsigned int& year, const unsigned int& month, const unsigned int& day)
{
    tm date;

    if (month > 12) {
        cout << "Month can not be >12";
        exit(1);
    }

    if (day > 31) {
        cout << "Day can not be >31";
        exit(1);
    }

    date.tm_year = (int)year - 1900;
    date.tm_mon  = month - 1;
    date.tm_mday = day;

    return date;
}

class Human
{
protected:
    string first_name, last_name;
    tm birth_date;

    void setBirthDate(tm birth_date)
    {
        this->birth_date = birth_date;
    }
public:
    Human(const string &first_name, const string &last_name, tm birth_date)
    {
        this->birth_date = birth_date;
        this->first_name = first_name;
        this->last_name  = last_name;   
    }

    // -------
    // Getters
    const string& getLastName()const
    {
        return last_name;
    }

    const string& getFirstName()const
    {
        return first_name;
    }

    const tm& getBirthDate()const
    {
        return birth_date;
    }
    // -------
    // Setters
    void setLastName(const string& last_name)
    {
        this->last_name = last_name;
    }

    void setFirstName(const string& first_name)
    {
        this->first_name = first_name;
    }

    unsigned int age()const
    {
        time_t now = time(0);
        tm* tm_now = new tm;
        localtime_s(tm_now, &now);

        unsigned int age = tm_now->tm_year - this->birth_date.tm_year;

        age -= 
            tm_now->tm_mon < this->birth_date.tm_mon 
            || tm_now->tm_mon == this->birth_date.tm_mon && tm_now->tm_mday < this->birth_date.tm_mday;

        return age;
    }
};

class Student : public Human
{
    string speciality;
    string group;
    int rating;
    int attendance;
public:
    Student(
        const string& first_name, const string& last_name, tm birth_date, 
        string speciality, string group, int rating, int attendance
    ) : Human(first_name, last_name, birth_date)
    {
        this->setGroup(group);
        this->setRating(rating);
        this->setSpeciality(speciality);
        this->setAttendance(attendance);
    }

    Student(
        const Human& human, string speciality, string group, 
        int rating, int attendance
    ): Human(human)
    {
        this->setGroup(group);
        this->setRating(rating);
        this->setSpeciality(speciality);
        this->setAttendance(attendance);
    }
    
    void setSpeciality(string speciality)
    {
        this->speciality = speciality;
    }

    void setGroup(string group)
    {
        this->group = group;
    }

    void setRating(int rating)
    {
        this->rating = rating;
    }

    void setAttendance(int attendance)
    {
        this->attendance = attendance;
    }
};

int main()
{
    Human h("Alexandr", "Shvedov", humanBirthDate(2005, 4, 17));

    Student hStudent(h, "IT", "BV_225", 12, 100);
    cout << h.age();
}