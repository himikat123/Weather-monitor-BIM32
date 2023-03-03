import React from "react";
import "../node_modules/bootstrap/dist/js/bootstrap.js";
import "./scss/style.scss";

class AppFn extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            configState: {
                error: null,
                isLoaded: false
            },
            config: {},

            alarmState: {
                error: null,
                isLoaded: false
            },
            alarm: {},

            dataState: {
                error: null,
                isLoaded: false
            },
            data: {},
            pass: {
                old: "",
                new: ""
            },
            path: "",
            gitVersion: null,
            newVersion: false
        };

        this.getConfig = this.getConfig.bind(this);
        this.getAlarmConfig = this.getAlarmConfig.bind(this);
        this.getData = this.getData.bind(this);
        this.changedConfig = this.changedConfig.bind(this);
        this.changedAlarm = this.changedAlarm.bind(this);
        this.changedPass = this.changedPass.bind(this);
    };

    /**
     * gets config from json file
     */
    getConfig() {
        fetch("./config.json")
        .then(res => res.json())
        .then(
            (result) => {
                this.setState({ config: result });
                if(result.hasOwnProperty('sensors')) {
                    this.setState({ configState: { isLoaded: true } });
                    this.getData();
                }
            },
            (error) => {
                this.setState({ configState: { isLoaded: true, error }, config: { lang: 'en' } });
            }
        )
        .then(() => {
            if(!this.state.configState.isLoaded || this.state.configState.error) {
                setTimeout(() => {
                    //this.setState({ config: { lang: 'en' } });
                    this.getConfig();
                }, 5000);
            }
        })
        fetch("https://raw.githubusercontent.com/himikat123/Weather-monitor-BIM32/master/BIM32/globals.hpp")
        .then((response) => response.text())
        .then((text) => {
            let regex = /] = "(v.+)"/gm;
            this.setState({ gitVersion: regex.exec(text)[1] });
        });
    }

    /**
     * gets alarm config from json file
     */
     getAlarmConfig() {
        fetch("./alarm.json")
        .then(res => res.json())
        .then(
            (result) => {
                this.setState({
                    alarmState: { isLoaded: true }, 
                    alarm: result
                });
            },
            (error) => {
                this.setState({ alarmState: { isLoaded: true, error } });
            }
        )
        .then(() => {
            if(!this.state.alarmState.isLoaded || this.state.alarmState.error) {
                setTimeout(() => {
                    this.getAlarmConfig();
                }, 5000);
            }
        })
    }

    /**
     * get data from host
     */
    getData() {
        if(window.location.pathname != '/firmware') {
            fetch("./data.json")
                .then(res => res.json())
                .then((result) => {
                    this.setState({
                        dataState: { isLoaded: true }, 
                        data: result
                    });
                    if(this.state.gitVersion) {
                        if(result.fw != this.state.gitVersion) this.setState({ newVersion: true });
                    }
                },
                (error) => {
                    this.setState({ dataState: { isLoaded: true, error } });
                }
            ).then(() => {
                setTimeout(() => { 
                    this.getData();
                }, 5000);
            });
        }
        else setTimeout(() => { 
            this.getData();
        }, 1000);
    }

    componentDidMount() {
        this.getConfig();
        this.getAlarmConfig();
    };

    changedConfig(newConfig) {
        this.setState({ config: newConfig });
    }

    changedAlarm(newConfig) {
        this.setState({ alarm: newConfig });
    }

    changedPass(oldPass, newPass) {
        this.setState({ pass: {old: oldPass, new: newPass} });
    }

    /**
     * 
     */
    loginError(error) {
        if(!error) window.location.assign("/");
    }
};

export default AppFn;