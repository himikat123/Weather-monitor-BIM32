import React from "react";
import RangeInput from "./RangeInput";

class Wsensor extends React.Component {
    render() {
        return (
        <div className="row align-items-center">
            <div className="col col-12 col-sm-5">
                <p className="text-center my-2">
                    {this.props.name}<br /><span className="indication">{this.props.value}</span>
                </p>
            </div>
            <div className="col col-12 col-sm-7">
                <RangeInput value={this.props.corr}
                    min={this.props.min}
                    max={this.props.max}
                    limitMin={this.props.min}
                    limitMax={this.props.max}
                    step={this.props.step}
                    config={this.props.config} 
                    changedConfig={this.props.changedConfig}
                    units={this.props.units}
                />
            </div>
        </div>)
    }
};

export default Wsensor;