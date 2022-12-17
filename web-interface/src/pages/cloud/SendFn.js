import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class SendFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            mac: "--",
            getCoordinates: '',
            menuOpen: 1
        }

        this.changedConfig = this.changedConfig.bind(this);
        this.getGeolocation = this.getGeolocation.bind(this);
    }

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps) {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }

        if(nextProps.data.hasOwnProperty('network')){
            let mac = nextProps.data.network.mac;
            if(mac != undefined) mac = mac.replace(/:/g, "");
            if(mac != this.state.mac) this.setState({ mac: mac });
        }
    }

    /**
     * Get Geolocation from browser
     */
    getGeolocation() {
        this.setState({ getCoordinates: 'spinner-border spinner-border-sm ms-2' });
        if(navigator.geolocation) {
            navigator.geolocation.getCurrentPosition((position) => {
                this.setState({ getCoordinates: '' });
                this.props.config.narodmonSend.lat = position.coords.latitude;
                this.props.config.narodmonSend.lon = position.coords.longitude;
                this.changedConfig(this.props.config);
            });
        }
    }
};

export default SendFn;