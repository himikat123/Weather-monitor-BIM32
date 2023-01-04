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

then in file **/built/index.html** move the ***&lt;script&gt;...&lt;/script&gt;*** tag along with its contents between tags ***&lt;/body&gt;*** and ***&lt;/html&gt;***

and then run

        $ gulp