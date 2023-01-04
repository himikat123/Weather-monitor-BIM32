# Start debugging
        $ cd .\web-interface
        $ npm start


## To swith to login page
in file /public/data.json set 

    "state": "LOGIN"


## To switch to other pages
in file /public/data.json set

    "state": "OK"


# Start debug server (in another terminal)
        $ cd .\web-interface\server\
        $ node serverRun


## or
# Build project
        $ cd .\web-interface
        $ npm run build

in file /built/index.html <script>...</script> move in between </body> and </html>
and then run

        $ gulp