# electronics-laboratory

## Building latex source
1) Setup environment variables for private data: copy the `.env.template`  file
   and update it with the correct data.
   ```bash
   cp sessions/.env.template sessions/.env
   ```
2) `cd` to lab session directory
    ```bash
    cd sessions/session-name
   ```
3) run
   ```bash
   pipenv run build:report
   ```
   to compile latex files once, or
   ```bash
   pipenv run watch:report
   ```
   to build latex on file change.
4) Hope you have all the needed dependencies. Otherwise, `tlmgr` is your
   friend :).
5) The pdf file will be generated in the `build` folder inside the session
   folder.
