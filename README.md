# Introduction
This is a very basic C intro type project. Compile it and run the program with a URL argument passed to the CLI, it will fetch the web server's response headers as well as the response body and then output the two data points to the console.

# Usage
Compile:
```
gcc url-content-fetcher.c -o url-content-fetcher.exe -lcurl
```

Execute
```
./url-content-fetcher.exe <url> <request_type>
```

# Notes
N/A

# Requirements & Dependencies
- ANSI-C
- cURL library
