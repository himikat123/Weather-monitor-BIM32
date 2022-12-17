import ConfigValue from "./ConfigValue";

class Save extends ConfigValue {
    constructor(props) {
        super(props);
        this.state = {sending: false}

        this.sendGetRequest = this.sendGetRequest.bind(this);
    }

    /**
     * Save new config
     * @param {*} newConfig 
     */
    changedConfig(newConfig) {
        this.props.changedConfig(newConfig);
    }

    /**
     * Send data via GET request
     */
    async sendGetRequest(destination, data) {
        if(!this.state.sending) {
            let origin = window.location.origin;
            let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
            await fetch(`${href}/esp/${destination}?${encodeURI(data)}`, 
                { mode: 'cors' }
            )
            .then(res => res.json())
            .then(() => { this.setState({ sending: true })})
            .then(() => { 
                setTimeout(() => {
                    this.setState({ sending: false });
                }, 1000);
            })
        }
    }
};

export default Save;