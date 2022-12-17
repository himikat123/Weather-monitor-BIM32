import React from "react";
import Languages from "../Languages";

class ModalNetworkList extends React.Component {
    constructor(props) {
        super(props);
        this.state = { data: {} };
        this.generateNetworksList = this.generateNetworksList.bind(this);
    };

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps) {
        if(this.state.data != nextProps.data) this.setState({data: nextProps.data});
    }

    /**
     * SSID from list has been selected
     * @param {*} net 
     */
    addNet(net) {
        this.props.addNet(net);
        document.querySelector('#closeNet' + this.props.num).click();
    }

    /**
     * generates network list
     * @returns list
     */
    generateNetworksList() {
        let list = [];
        for(let network in this.state.data.ssids) {
            list.push(<div key={network} className="d-flex justify-content-between net-list px-2" onClick={() => {this.addNet(this.state.data.ssids[network][0])}}>
                <div>{this.state.data.ssids[network][0]}</div>
                <div title={`-${this.state.data.ssids[network][1]}dB`}>
                    <svg x="0px" y="0px" viewBox="0 0 494.45 494.45" width="20px" className="net-svg">
                        {this.state.data.ssids[network][1] < 30 && <path d="M467.925,204.625c-6.8,0-13.5-2.6-18.7-7.8c-111.5-111.4-292.7-111.4-404.1,0c-10.3,10.3-27.1,10.3-37.4,0 s-10.3-27.1,0-37.4c64-64,149-99.2,239.5-99.2s175.5,35.2,239.5,99.2c10.3,10.3,10.3,27.1,0,37.4 C481.425,202.025,474.625,204.625,467.925,204.625z"/>}
                        {this.state.data.ssids[network][1] < 60 && <path d="M395.225,277.325c-6.8,0-13.5-2.6-18.7-7.8c-71.4-71.3-187.4-71.3-258.8,0c-10.3,10.3-27.1,10.3-37.4,0 s-10.3-27.1,0-37.4c92-92,241.6-92,333.6,0c10.3,10.3,10.3,27.1,0,37.4C408.725,274.725,401.925,277.325,395.225,277.325z"/>}
                        {this.state.data.ssids[network][1] < 90 && <path d="M323.625,348.825c-6.8,0-13.5-2.6-18.7-7.8c-15.4-15.4-36-23.9-57.8-23.9s-42.4,8.5-57.8,23.9	c-10.3,10.3-27.1,10.3-37.4,0c-10.3-10.3-10.3-27.1,0-37.4c25.4-25.4,59.2-39.4,95.2-39.4s69.8,14,95.2,39.5 c10.3,10.3,10.3,27.1,0,37.4C337.225,346.225,330.425,348.825,323.625,348.825z"/>}
                        <circle cx="247.125" cy="398.925" r="35.3"/>
                    </svg>
                </div>
            </div>);
        }
        return list;
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return(
            <div className="modal fade" id={"netList" + this.props.num} tabIndex="-1">
                <div className="modal-dialog">
                    <div className="modal-content">
                        <div className="modal-header">
                            <h5 className="modal-title">{text.get('listOfAvailableNetworks', lang)}</h5>
                            <button type="button" className="btn-close" data-bs-dismiss="modal" title={text.get('close', lang)}></button>
                        </div>
                        <div className="modal-body">
                            {this.generateNetworksList().length == 0 && <button className="btn btn-light" type="button" disabled>
                                {text.get('loadingListOfAvailableNetworks', lang)}
                                <span className="spinner-border spinner-border-sm" role="status"></span>
                            </button>}
                            {this.generateNetworksList()}
                        </div>
                        <div className="modal-footer">
                            <button type="button" id={"closeNet" + this.props.num} className="btn btn-secondary" data-bs-dismiss="modal">
                                {text.get('close', lang)}
                            </button>
                        </div>
                    </div>
                </div>
            </div>
        );
    }
};
export default ModalNetworkList;