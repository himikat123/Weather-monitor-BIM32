import Save from "../pages/Save";

class IpField extends Save{
    constructor(props){
        super(props);
        this.state = {error: ''};
        this.changedConfig = this.changedConfig.bind(this);
        this.validate = this.validate.bind(this);
        this.changedIp = this.changedIp.bind(this);
    };

    /**
     * validate the entered IP address
     * @param {*} event 
     */
    validate(event){
        let reg = /\b(([01]?\d?\d|2[0-4]\d|25[0-5])\.){3}([01]?\d?\d|2[0-4]\d|25[0-5])\b/;
        let ip = event.target.value;
        if(ip.search(reg) == -1) this.setState({error: ' bg-danger'});
        else this.setState({error: ''});
    }

    /**
     * save changed ip address
     * @param {*} event 
     */
    changedIp(event){
        let config = this.setVal(this.props.config, this.props.value, event.target.value);
        this.changedConfig(config);
        this.validate(event);
    }

    render(){
        return (
            <div className="mb-3 mt-3 position-relative">

                <label className="form-label">{this.props.label}</label>

                {/* Dynamic IP */}
                {this.props.config.hasOwnProperty("network") && 
                 this.props.data && this.props.data.hasOwnProperty("network") &&
                 this.props.config.network.type == 0 && 
                    <input type="text" 
                        className="form-control bg-secondary" 
                        defaultValue={this.props.data.network[this.props.valueDyn]} 
                        disabled 
                    />
                }

                {/* Static IP */}
                {this.props.config.hasOwnProperty("network") &&
                 this.props.config.network.type == 1 && 
                    <input type="text" 
                        className={'form-control' + this.state.error}  
                        placeholder={this.props.label} 
                        value={this.getVal(this.props.config, this.props.value)} 
                        onChange={this.changedIp} 
                        maxLength="15" 
                    />
                }
            </div>
        );
    };
};

export default IpField;