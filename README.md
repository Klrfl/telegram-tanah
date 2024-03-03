# Monitor tanah

Ini adalah bot Telegram untuk memonitor tingkat kelembaban tanah. Nilai kelembaban akan dikirim lewat Telegram.
Skrip dikembangkan untuk ESP8266.

## Pengembangan

Clone repo ini, lalu masuk ke dalamnya. Setelah itu salin file `CredentialsExample.h` dan beri nama `Credentials.h`.
Setelah itu, ubah `CHAT_ID`, `BOT_ID`, `SSID` dan `PASSWORD` sesuai kebutuhan.

Jika menggunakan Linux, bisa menggunakan command-command berikut:

```sh
git clone https://github.com/Klrfl/telegram-tanah.git

# jika menggunakan SSH
git clone git@github.com:Klrfl/telegram-tanah.git

cd telegram-tanah

cp CredentialsExample.h Credentials.h
```

Jika sudah maka unggah kode ke ESP8266.