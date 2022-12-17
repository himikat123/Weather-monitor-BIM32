import Save from "../Save";
import axios from 'axios';
import Languages from "../../Languages";
import { closeMenu } from "../../menu/closeMenu";

class ComfortFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            sendRequest: '...', 
            receiveRequest: '...', 
            spinnerNTP: '', 
            spinnerPC: '',
            spinnerManually: '',
            clockSync: false,
            manualTime: 0,
            menuOpen: 1
        };
        
        this.changedConfig = this.changedConfig.bind(this);
        this.clockSyncNTP = this.clockSyncNTP.bind(this);
        this.clockSyncPC = this.clockSyncPC.bind(this);
        this.timeManuallyChanged = this.timeManuallyChanged.bind(this);
        this.clockSyncManually = this.clockSyncManually.bind(this);
    };

    /**
     * Clock sync with NTP server
     * sends request
     */
    clockSyncNTP() {
        let text = new Languages();
        let lang = this.props.config.lang;
        let sync = 0;
        // show spinner and synchronizing message
        this.setState({ 
            spinnerNTP: 'spinner-border spinner-border-sm ms-2',
            sendRequest: text.get('synchronizingNTP', lang),
            receiveRequest: '' 
        });
        // send NTP sync command to the host
        this.sendGetRequest('syncClock', 'ntp=1');

        /*
        * checking the status of synchronization with NTP
        */
        let syncdialog = setInterval(() => {
            sync++;
            // if more than 10 attempts stop the synchronization process
            // and show error message
            if(sync > 10){
                this.setState({ 
                    spinnerNTP: '',
                    receiveRequest: text.get('ntpSyncError', lang) 
                });
                clearInterval(syncdialog);
            }
            // requesting synchronization status
            let origin = window.location.origin;
            let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
            axios(`${href}/esp/syncdialog`)
            .then(result => {
                if(result.data == '.') this.setState({ sendRequest: this.state.sendRequest + '.' });
                else{
                    this.setState({ 
                        spinnerNTP: '',
                        receiveRequest: text.get('synchronized', lang) + ' ' + result.data 
                    });
                    clearInterval(syncdialog);
                }
            });
        }, 1000);
    }

    /**
     * Clock sync with PC
     * sends request and data
     */
    clockSyncPC() {
        let date = new Date();
        let time = date.toISOString().split('T')[0] + ' ' + date.toLocaleTimeString();
        // show spinner and current time
        this.setState({
            spinnerPC: 'spinner-border spinner-border-sm ms-2',
            sendRequest: String(time) 
        });
        // send browser time to host 
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios(`${href}/esp/syncClock?y=${date.getFullYear()}&m=${date.getMonth()+1}&d=${date.getDate()}&h=${date.getHours()}&i=${date.getMinutes()}&s=${date.getSeconds()}`)
        .then(result => {
            this.setState({ 
                receiveRequest: result.data, 
                spinnerPC: '' 
            }) 
        });
    }

    /**
     * Time in manual set changed
     */
    timeManuallyChanged(event) {
        let dateTime = event.target.value;
        this.setState({ manualTime: dateTime }); 
    }

    /**
     * Clock set manual
     * sends request and data
     */
    clockSyncManually() {
        // show spinner and desired time
        this.setState({
            spinnerManually: 'spinner-border spinner-border-sm ms-2',
            sendRequest: this.state.manualTime.replace('T', ' ')
        });
        // send desired time to host 
        let tm = this.state.manualTime.replace(/[T-]/g, ':').split(':');
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios(`${href}/esp/syncClock?y=${tm[0]}&m=${tm[1]}&d=${tm[2]}&h=${tm[3]}&i=${tm[4]}&s=${tm[5]}`)
        .then(result => {
            this.setState({ 
                receiveRequest: result.data, 
                spinnerManually: '' 
            }) 
        });
    }

    
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }
};

export default ComfortFn;