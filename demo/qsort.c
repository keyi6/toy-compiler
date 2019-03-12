int a[10] = {3, 5, 2, 4, 1, 6}, n;

void qsort(int l, int r) {
    if (l < r) {
        int i, j, base;

        base = a[l];
        i = l;
        j = r;

        while (i < j) {
            while (a[j] >= base && i < j) {
                j = j - 1;
            }
            a[i] = a[j];

            while (a[i] <= base && i < j) {
                i = i + 1;
            }
            a[j] = a[i];
        }

        a[i] = base;

        qsort(l, i - 1);
        qsort(i + 1, r);
    }
}

void main() {
    n = 6;
    qsort(0, n - 1);

    int i;
    for (i = 0; i < n; i = i + 1) {
        print(a[i]);
    }
}