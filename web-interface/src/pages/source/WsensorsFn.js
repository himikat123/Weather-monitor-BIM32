import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";

class WsensorsFn extends Save{
    constructor(props){
        super(props);
        this.state = { data: {}, menuOpen: 1 };
        this.changedConfig = this.changedConfig.bind(this);
        this.getName = this.getName.bind(this);
        this.getTime = this.getTime.bind(this);
        this.getTemp = this.getTemp.bind(this);
        this.getVal = this.getVal.bind(this);
    };

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps){
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }

        if(this.state.data != nextProps.data) this.setState({data: nextProps.data});
    }

    /**
     * Get wireless sensor name
     * @param {*} num number of wireless sensor 
     * @returns name or "--" if not exists
     */
    getName(num) {
        try { return this.state.data.wsensor.name[num] }
        catch(e) { return '--' }
    }

    /**
     * Get last update time of wireless sensor
     * @param {*} num number of wireless sensor 
     * @returns date & time or "--" if not exists
     */
    getTime(num) {
        try { return this.state.data.wsensor.time[num] }
        catch(e) { return '--' }
    }

    /**
     * Get wireless sensor temperature
     * @param {*} num number of wireless sensor
     * @param {*} corr correction factor
     * @param {*} tnum number of temperature 
     * @returns temperature or "--" if not exists
     */
    getTemp(num, corr, tnum) {
        try {
            let temp = Number(this.state.data.wsensor.temp.data[tnum][num]);
            if(temp < -55 || temp > 100) return '--';
            return ((Math.round((temp + Number(corr)) * 10) / 10) + '°C, ' + 
                this.state.data.wsensor.temp.name[tnum][num]
            );
        }
        catch(e) { return '--' }
    }

    /**
     * Get a value from wireless sensor
     * @param {*} num number of wireless sensor 
     * @param {*} corr correction factor
     * @param {*} units units
     * @param {*} sens type of sensor
     * @param {*} min minimum allowable value
     * @param {*} max maximum allowable value
     * @returns value or "--" if not exists
     */
    getVal(num, corr, units, sens, min, max, round) {
        try {
            let val = Number(this.state.data.wsensor[sens].data[num]);
            if(val < min || val > max) return '--'; 
            return ((Math.round((val + Number(corr)) * round) / round) + units + ', ' +
                this.state.data.wsensor[sens].name[num]
            );
        }
        catch(e) { return '--' }
    }

    /**
     * Get a battery voltage from wireless sensor
     * @param {*} num number of wireless sensor
     * @param {*} units units
     * @returns voltage or "--" if not exists
     */
    getBat(num, units) {
        try {            
            let val = Number(this.state.data.wsensor.bat[num]);
            if(val < 0 || val > 1023) return '--'; 
            return (Math.round(
                (this.props.data.wsensor.bat[num] / (300 - this.props.config.wsensor.bat.k[num])) * 1000
            ) / 1000 + ' ' + units);
        }
        catch(e) { return '--' }
    }
};

export default WsensorsFn;