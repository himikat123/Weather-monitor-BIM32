import React from "react";

function WeatherRow(props) {
    return (
        <div className="row">
            <div className="col-6 text-end">{props.text}:</div>
            <div className={"col-6 " + props.color}>{props.value}</div>
        </div>
    );
}

export default WeatherRow;