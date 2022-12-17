# start debugging
$ cd .\web-interface
$ npm start

## to swith to login page
in file /public/data.json set "state": "LOGIN"
## to switch to other pages
in file /public/data.json set "state": "OK"


# start debugging server (in another terminal)
$ cd .\web-interface\server\
$ node serverRun


## or
# build project
$ cd .\web-interface
$ npm run build
in file /built/index.html <script>...</script> move in between </body> and </html>
gulp