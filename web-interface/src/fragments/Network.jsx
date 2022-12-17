import Save from "../pages/Save";
import Languages from "../Languages";
import PasswordField from "./PasswordField";
import ModalNetworkList from "../modals/ModalNetworkList";

class Network extends Save{
    constructor(props){
        super(props);
        this.state = {data: {}, ssid: ''};
        this.getSSID = this.getSSID.bind(this);
        this.SsidChanged = this.SsidChanged.bind(this);
        this.changedConfig = this.changedConfig.bind(this);
        this.addNet = this.addNet.bind(this);
    };

    /**
     * checks if SSID exists and
     * @returns SSID or empty string 
     */
    getSSID(){
        let ssid;
        try{ ssid = this.props.config.network.ssid[this.props.num]; }
        catch(e){}
        return ssid != undefined ? ssid : '';
    }

    /**
     * save changed SSID
     * @param {*} event 
     */
    SsidChanged(event){
        this.props.config.network.ssid[this.props.num] = event.target.value;
        this.props.changedConfig(this.props.config);
    }

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps){
        if(this.state.data != nextProps.data) this.setState({data: nextProps.data});
        if(this.state.ssid != this.getSSID()) this.setState({ssid: this.getSSID()});
    }

    /**
     * SSID from list has been selected
     * @param {*} net 
     */
    addNet(net){
        this.props.config.network.ssid[this.props.num] = net;
        this.setState({ssid: net});
    }

    render(){
        let text = new Languages();
        let lang = this.props.config.lang;

        return(
            <div className="col border border-secondary rounded m-4 p-3 tile">

                <h4 className="d-flex justify-content-center">
                    {text.get('network', lang)} {Number(this.props.num) + 1}
                </h4><hr />

                <label className="form-label">{text.get('networkName', lang)}</label>

                <div className="input-group mb-3">

                    <input type="text" 
                        className="form-control" 
                        placeholder={text.get('networkName', lang)}  
                        defaultValue={this.state.ssid} 
                        onChange={this.SsidChanged}
                        maxLength="32" 
                    />

                    <button className="input-group-text" 
                            title={text.get('listOfAvailableNetworks', lang)} 
                            data-bs-toggle="modal" 
                            data-bs-target={"#netList" + this.props.num}>
                        <svg viewBox="0 0 30 30" width="23px">
                            <path stroke="rgba(0, 0, 0)" strokeLinecap="round" strokeMiterlimit="10" strokeWidth="2" d="M4 7h22M4 15h22M4 23h22" />
                        </svg>
                    </button>
                </div>

                <PasswordField label={text.get('password', lang)} 
                    num={this.props.num} 
                    config={this.props.config} 
                    changedConfig={this.changedConfig} 
                />

                <ModalNetworkList num={this.props.num} 
                    config={this.props.config} 
                    data={this.state.data} 
                    addNet={this.addNet} 
                />
            </div>
        );
    }
};
export default Network;