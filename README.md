# Campus Analytics Engine

BS AI - Programming Fundamentals Project  
A menu-driven console system for managing students, courses, attendance, grades, and fees.

## Features

- **Students** - add, search, update, soft-delete, list active, search-as-you-type (bonus)
- **Courses** - enroll/drop, credit load check, prerequisite validation
- **Attendance** - mark P/A/L, compute %, shortage list, undo last session
- **Grades** - enter marks, GPA, class statistics, attendance penalty
- **Fees** - record payments, late fines, receipts, defaulters
- **Reports** - merit list, defaulters, semester results, department summary, export

## File Structure

```
main.cpp
filehandler.h / filehandler.cpp
student_ops.h / student_ops.cpp
course_ops.h / course_ops.cpp
attendance.h / attendance.cpp
grades.h / grades.cpp
fee_tracker.h / fee_tracker.cpp
reports.h / reports.cpp
Data/
  students.txt, courses.txt, enrollments.txt
  attendance_log.txt, fees.txt, grades.txt
```

## How to Compile

```bash
g++ -std=c++17 main.cpp filehandler.cpp student_ops.cpp course_ops.cpp attendance.cpp grades.cpp fee_tracker.cpp reports.cpp -o campus
```

## How to Run

Run from the project folder so `Data/` paths work:

```bash
./campus
```

## Sample Run

```
=== MAIN MENU ===
1. Students
2. Courses
...
6. Reports -> 1. Merit List
```

Search student: Main Menu -> 1 -> 2 -> enter `BSAI-23-001`

## Notes

- Roll format: `BSAI-YY-XXX`
- Dates: `DD-MM-YYYY`
- Uses struct, loops, file I/O only (no STL algorithms, no class, no ctime)
- Data files copied from practice dataset in `first-project/Data`

## Author

BS AI - 2nd Semester PF Project
