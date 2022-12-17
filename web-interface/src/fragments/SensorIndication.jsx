import React from "react";
import RangeInput from "./RangeInput";

class SensorIndication extends React.Component{
    constructor(props){
        super(props);
        this.state = {data: {}};
    }

    render(){
        return(
            <div className="mt-4 w-100">
                <div className="mb-2 text-center">
                    {this.props.name}: <span className="text-primary indication me-3">{this.props.data}{this.props.units}</span>
                </div>

                <RangeInput value={this.props.corr}
                    min="-10"
                    max="10"
                    limitMin="-10"
                    limitMax="10"
                    step="0.1"
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig}
                    units={this.props.units}
                />
            </div>
        );
    }
};

export default SensorIndication;