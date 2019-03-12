void judge(int year) {
    if (year % 4 == 0 && year % 100 != 0 || year % 400 == 0) {
        print(year, "is leap year.");
    }
    else {
        print(year, "is not leap year.");
    }
}


void main() {
    judge(1900);
    judge(2000);
}

