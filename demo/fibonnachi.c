void main() {
    int i, f[10] = {1, 1};

    for (i = 2; i < 9; i = i + 1) {
        f[i] = f[i - 1] + f[i - 2];
    }

    for (i = 0; i < 9; i = i + 1) {
        print(f[i]);
    }
}
