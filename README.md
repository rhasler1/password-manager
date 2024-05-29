### Example use:
1. set universal key and master key
    - ./pmanager -s -k cyndy -m luci

2. add entry
    - ./pmanager -a -k cyndy -m luci --id gmail --user test@gmail.com --pass testing

3. fetch entry
    - ./pmanager -f -k cyndy -m luci --id gmail

    - output: <username;password> = test@gmail.com;testing