#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struct data
typedef struct pasien {
    int id;
    char nama[100];
    int usia;
    char keluhan[100];
    char diagnosis[100];
    struct pasien* prev;
    struct pasien* next;
} pasien;

// pointer untuk dll
pasien *head = NULL;
pasien *tail = NULL;
int autoId = 1;

int main() {
    // header CGI nya
    printf("Content-Type: text/html\n\n");
    printf("<html><body>\n");

    // metode CGI
    char *metode = getenv("REQUEST_METHOD");
    
    // POST
    if (strcmp(metode, "POST") == 0) {
        char *panjang_input_str = getenv("CONTENT_LENGTH");
        int panjang_input = atoi(panjang_input_str);
        char data[1000];
        fgets(data, panjang_input + 1, stdin);

        // 1. tambah data pasien
        if (strstr(data, "nama=")) {
            char nama[100], keluhan[100], diagnosis[100];
            int usia;
            sscanf(data, "nama=%[^&]&usia=%d&keluhan=%[^&]&diagnosis=%s",
                   nama, &usia, keluhan, diagnosis);

            pasien *baru = (pasien*)malloc(sizeof(pasien));
            baru->id = autoId++;
            strcpy(baru->nama, nama);
            baru->usia = usia;
            strcpy(baru->keluhan, keluhan);
            strcpy(baru->diagnosis, diagnosis);
            baru->prev = tail;
            baru->next = NULL;

            if (head == NULL)
                head = tail = baru;
            else {
                tail->next = baru;
                baru->prev = tail;
                tail = baru;
            }

            printf("<h3>Data pasien berhasil ditambahkan.</h3>");
            printf("<p>ID: %d<br>Nama: %s</p>", baru->id, nama);
        }

        // 2. hapus data pasien
        else if (strstr(data, "hapus_id=")) {
            int id_target;
            sscanf(data, "hapus_id=%d", &id_target);
            pasien *hapus = head;

            while (hapus != NULL) {
                if (hapus->id == id_target) {
                    if (hapus == head && hapus == tail)
                        head = tail = NULL;
                    else if (hapus == head) {
                        head = hapus->next;
                        head->prev = NULL;
                    } else if (hapus == tail) {
                        tail = hapus->prev;
                        tail->next = NULL;
                    } else {
                        hapus->prev->next = hapus->next;
                        hapus->next->prev = hapus->prev;
                    }
                    free(hapus);
                    printf("<h3>Pasien ID %d berhasil dihapus.</h3>", id_target);
                    break;
                }
                hapus = hapus->next;
            }
        }

        printf("<br><a href='../html/meditoria.html'> Kembali ke Beranda</a>");
    }

    // 3. lihat data pasien + GET
    else if (strcmp(metode, "GET") == 0) {
        char *query = getenv("QUERY_STRING");

        if (query && strstr(query, "lihat=1")) {
            printf("<h2>Daftar Data Pasien</h2>");
            printf("<table border='1'>");
            printf("<tr><th>ID</th><th>Nama</th><th>Usia</th><th>Keluhan</th><th>Diagnosis</th><th>Aksi</th></tr>");

            pasien *bantu = head;
            while (bantu != NULL) {
                printf("<tr>");
                printf("<td>%d</td><td>%s</td><td>%d</td><td>%s</td><td>%s</td>",
                       bantu->id, bantu->nama, bantu->usia, bantu->keluhan, bantu->diagnosis);
                printf("<td>");
                printf("<form method='POST' action='/cgi-bin/pasien.cgi'>");
                printf("<input type='hidden' name='hapus_id' value='%d'>", bantu->id);
                printf("<input type='submit' value='Hapus'>");
                printf("</form>");
                printf("</td></tr>");
                bantu = bantu->next;
            }

            printf("</table><br>");
            printf("<a href='../html/meditoria.html'> Kembali ke Beranda</a>");
        }
    }

    printf("</body></html>");
    return 0;
}
