import Languages from "../Languages";
import axios from "axios";
import React from "react";

class AlarmBlockFn extends React.Component {
    constructor(props) {
        super(props);

        this.state = { 
            hr: "00", mn: "00", state: 0,
            mo: 0, tu: 0, we: 0, th: 0,
            fr: 0, sa: 0, su: 0, melody: 1, 
            loading: true
        }

        this.changedTime = this.changedTime.bind(this);
        this.changedOnOff = this.changedOnOff.bind(this);
        this.changedWd = this.changedWd.bind(this);
        this.changedMelody = this.changedMelody.bind(this);
        this.play = this.play.bind(this);
    }

    /**
     * props have been updated
     * @param {*} nextProps 
     */
    componentDidUpdate(nextProps) {
        if(JSON.stringify(nextProps.alarm) !== '{}') {
            if(this.state.loading) this.setState({ loading: false });
            let hr = nextProps.alarm.time[this.props.num][0],
            mn = nextProps.alarm.time[this.props.num][1],
            state = nextProps.alarm.states[this.props.num],
            mo = nextProps.alarm.weekdays[this.props.num][0],
            tu = nextProps.alarm.weekdays[this.props.num][1],
            we = nextProps.alarm.weekdays[this.props.num][2],
            th = nextProps.alarm.weekdays[this.props.num][3],
            fr = nextProps.alarm.weekdays[this.props.num][4],
            sa = nextProps.alarm.weekdays[this.props.num][5],
            su = nextProps.alarm.weekdays[this.props.num][6],
            melody = nextProps.alarm.melodies[this.props.num];
            
            try {
                if(hr != this.state.hr || mn != this.state.mn || state != this.state.state ||
                mo != this.state.mo || tu != this.state.tu || we != this.state.we ||
                th != this.state.th || fr != this.state.fr || sa != this.state.sa || 
                su != this.state.su || melody != this.state.melody) { 
                    this.setState({ 
                        hr: hr, mn: mn, state: state,
                        mo: mo, tu: tu, we: we, th: th,
                        fr: fr, sa: sa, su: su, melody: melody
                    });
                }
            }
            catch (e) {}
        }
    }

    /**
     * save new alarm config
     * @param {*} newConfig 
     */
    changedAlarm(newConfig) {
        this.props.changedAlarm(newConfig);
    }

    /**
     * time has been changed
     * @param {*} event
     */
    changedTime(event) {
        let time = event.target.value.split(':');
        this.setState({ hr: time[0], mn: time[1] });
        this.props.alarm.time[this.props.num][0] = Number(time[0]);
        this.props.alarm.time[this.props.num][1] = Number(time[1]);
        this.changedAlarm(this.props.alarm);
    }

    /**
     * On/Off checkbox has been changed
     * @param {*} event
     */
    changedOnOff(event) {
        this.setState({ state: event.target.checked });
        this.props.alarm.states[this.props.num] = event.target.checked ? 1 : 0;
        this.changedAlarm(this.props.alarm);
    }

    /**
     * Weekday checkbox has been changed
     * @param {*} event
     */
    changedWd(event) {
        let weekdayNumber = event.target.id.split('_')[1];
        let weekday = event.target.classList[0];
        this.setState({ [weekday]: event.target.checked });
        this.props.alarm.weekdays[this.props.num][weekdayNumber] = event.target.checked ? 1 : 0;
        this.changedAlarm(this.props.alarm);
    }

    /**
     * Melody number has been changed
     * @param {*} event
     */
    changedMelody(event) {
        this.setState({ melody: event.target.value });
        this.props.alarm.melodies[this.props.num] = Number(event.target.value);
        this.changedAlarm(this.props.alarm);
    }

    /**
     * Generate weekdkay name td
     * @param {*} name 
     * @returns 
     */
    weekdayName(name) {
        let text = new Languages();
        let language = this.props.lang;
        return <td key={name}>{text.get(name, language)}</td>;
    }

    /**
     * Generate weekdkay name td
     * @param {*} name of weekday
     * @param {*} num number of weekday 0-Monday ... 6-Sunday
     * @returns 
     */
    weekdayCheckbox(name, num) {
        return(<td key={num}>
            <input id={'wd_' + num} 
                className={name + ' form-check-input'} 
                type="checkbox" 
                checked={this.state[name]} 
                onChange={this.changedWd} 
            />
        </td>);
    }

    /**
     * play changed melody
     */
    play() {
        axios({ url: `/esp/mp3play?folder=2&track=${this.state.melody}` })
    }

    /**
     * stop playing
     */
    stop() {
        axios({ url: '/esp/mp3stop' });
    }
};

export default AlarmBlockFn;