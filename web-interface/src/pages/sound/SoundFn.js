import Save from "../Save";
import Languages from "../../Languages";
import { closeMenu } from "../../menu/closeMenu";

class SoundFn extends Save {
    constructor(props) {
        super(props);
        this.state = {
            hourlySound: 0,
            alarmMelody: 0,
            menuOpen: 1
        }

        this.changedConfig = this.changedConfig.bind(this);
        this.changedVolume = this.changedVolume.bind(this);
        this.clickEQ = this.clickEQ.bind(this);
        this.changedHourlySound = this.changedHourlySound.bind(this);
        this.changedAlarmMelody = this.changedAlarmMelody.bind(this);
        this.changedHrFrom = this.changedHrFrom.bind(this);
        this.changedHrTo = this.changedHrTo.bind(this);
        this.playHourly = this.playHourly.bind(this);
        this.playAlarm = this.playAlarm.bind(this);
        this.stopPlaying = this.stopPlaying.bind(this);
        this.getHours = this.getHours.bind(this);
    }

    /**
     * props have been updated 
     */
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    /**
     * volume has been changed
     */
    changedVolume() {
        this.sendGetRequest('volume', 
            `vol=${this.props.config.sound.vol}`
        );
    }

    /**
     * equalizer button has been pressed
     * @param {*} event 
     */
    clickEQ(event) {
        this.props.config.sound.eq = Number(event.target.classList[0].split('-')[1]);
        this.changedConfig(this.props.config);
        this.sendGetRequest('equalizer', 
            `eq=${this.props.config.sound.eq}`
        );
    }

    /**
     * hourly sound has been changed
     * @param {*} event 
     */
    changedHourlySound(event) {
        this.setState({ hourlySound: event.target.value });
    }

    /**
     * alarm melody has been changed
     * @param {*} event 
     */
    changedAlarmMelody(event) {
        this.setState({ alarmMelody: event.target.value });
    }

    /**
     * hourly time "from" has been changed
     * @param {*} event 
     */
    changedHrFrom(event) {
        this.props.config.sound.hour.from = Number(event.target.value.split(':')[0]);
        this.changedConfig(this.props.config);
    }

    /**
     * hourly time "to" has been changed
     * @param {*} event 
     */
    changedHrTo(event) {
        this.props.config.sound.hour.to = Number(event.target.value.split(':')[0]);
        this.changedConfig(this.props.config);
    }

    /**
     * play hourly sound
     */
    playHourly() {
        this.sendGetRequest('mp3play', 
            `folder=1&track=${this.state.hourlySound}`
        );
    }

    /**
     * play alarm melody
     */
    playAlarm() {
        this.sendGetRequest('mp3play', 
            `folder=2&track=${this.state.alarmMelody}`
        );
    }

    /**
     * stop playing
     */
    stopPlaying() {
        console.log("stopp");
        this.sendGetRequest('mp3stop', 'stop=1');
    }

    /**
     * hours string formation
     * @param {*} hour 
     * @returns hours string
     */
    getHours(hour) {
        let text = new Languages();
        let lang = this.props.config.lang;
        let hourString = String(hour) + ' ';
        if(hour == 1 || hour == 21) hourString += text.get('hour1', lang);
        else if((hour > 1 && hour < 5) || hour == 22 || hour == 23) hourString += text.get('hour2_4', lang);
        else hourString += text.get('hour5_0', lang);
        return hourString;
    }
};

export default SoundFn;