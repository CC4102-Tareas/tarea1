

    srand((unsigned int)time(NULL));
    int i;
    float a = 500000.0;
    for (i=0;i<10000;i++)
        float flo = ((float)rand()/(float)(RAND_MAX)) * a;
    printf("%f\n", flo);
    return 0;
