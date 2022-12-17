import React from "react";
import { closeMenu } from "../../menu/closeMenu";

class StatusFn extends React.Component {
    constructor(props) {
        super(props);
        this.state = { menuOpen: 1 }
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
    }

    /**
     * Temperature validation and correction
     */
    temp(t, corr) {
        return t < -55 || t > 100 ? '--' : Math.round((t + corr) * 10) / 10;
    }

    /**
     * Humidity validation and correction
     */
    hum(h, corr) {
        return h < 0 || h > 100 ? '--' : Math.round((h + corr) * 10) / 10;
    }

    /**
     * Pressure validation and correction
     */
    pres(p, corr) {
        return p < 500 || p > 1500 ? '--' : Math.round((p + corr) * 10) / 10;
    }

    /**
     * Ambiend light validation
     */
    light(l, corr) {
        return l < 0 || l > 200000 ? '--' : Math.round((l + corr) * 10) / 10;
    }

    /**
     * Voltage validation
     */
    voltage(v, corr) {
        return v < 0 || v > 500 ? '--' : Math.round((v + corr) * 10) / 10;
    }
};

export default StatusFn;