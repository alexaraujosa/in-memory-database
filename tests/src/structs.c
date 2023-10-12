#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define CREATE_MASK(a, b) ((1u << ((b) - (a) + 1)) - 1) << (a)

// Lenght of the largest portuguese proper noun: 
// "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga Xavier "
// "Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha"
#define MAX_NAME_LEN 157

#define ID_OFFSET 27

// volatile unsigned createMask(unsigned a, unsigned b){
//    unsigned r = 0;
//    for (unsigned i=a; i<=b; i++) r |= 1 << i;

//    return r;
// }

typedef struct id {
    char header;     // 8 bits
    unsigned int id; // 9 digits - 27 bits
} ID, *Id;

uint64_t makeId(char header, int data) {
    if ((int)ceil(log2(data) + 1) != 9) return -1;

    return ((uint64_t)header << ID_OFFSET) | data;
}

ID parseID(uint64_t id) {
    return (ID){
        (char)(id >> ID_OFFSET), 
        (int)(id & (1 << ID_OFFSET) - 1)
    };
}


typedef struct user {
    uint64_t id;
    char name[MAX_NAME_LEN];
    int phone_number;
    uint8_t age;
    bool sex;
    uint16_t country_code;
    int account_creation;
    bool account_status;
} USER;

int main() {
    uint64_t id = ((uint64_t)'U' << ID_OFFSET) | 123456789;
    char wtf[MAX_NAME_LEN] = "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga "
    "Xavier Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha";
    int wtf_len = strlen(wtf) * sizeof(char) * 8;

    printf("WTF size: %d | %d\n", wtf_len, (int)floor(log2(wtf_len) + 1));


    char name[9] = "John Doe";
    USER u = {
        // &id,
        id,
        // BUG: Tf is wrong with this?
        // makeId('U', 123456789),
        "Manuel Maria Filipe Carlos Amélio Luís Miguel Rafael Gabriel Gonzaga "
        "Xavier Francisco de Assis Eugénio de Bragança Orleães Sabóia e Saxe-Coburgo-Gotha",
        123456789,
        255,
        1,
        351,
        123456,
        1
    };
    
    printf("U ADDR: %p\n", &u);
    fflush(stdout);

    ID pid = parseID(u.id);
    printf("PARSED ID: '%c' %d\n", pid.header, pid.id);
    printf("ID: '%c' %d\n", (char)(u.id >> ID_OFFSET), (int)(u.id & (1 << ID_OFFSET) - 1));
    
    printf("The user has the data %ld | %s | %d | %d | %d | %d | %d | %d\n", 
        u.id, u.name, u.phone_number, u.age, u.sex, u.country_code, u.account_creation, u.account_status
    );
    printf("The size of user is %ld bytes.\n", sizeof(USER));
    fflush(NULL);
    return 0; 
}