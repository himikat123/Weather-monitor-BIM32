import React from "react";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import AlarmBlock from "../../fragments/AlarmBlock";
import Menu from "../../menu/Menu";
import SaveRestart from "../SaveRestart";
import { closeMenu } from "../../menu/closeMenu";

class Alarm extends React.Component {
    constructor(props) {
        super(props);
        this.state = { menuOpen: 1 };

        this.changedAlarm = this.changedAlarm.bind(this);
    };

    /**
     * save new alarm config
     * @param {*} newConfig 
     */
    changedAlarm(newConfig) {
        this.props.changedAlarm(newConfig);
    }

    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (<>
            <Menu language={lang} />

            <div className="d-flex justify-content-center">
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('alarm', lang)} />
                    <div className="row">
                        {[...Array(12)].map((x, i) =>
                            <AlarmBlock lang={lang} 
                                key={i} 
                                num={i} 
                                alarm={this.props.alarm}
                                changedAlarm={this.changedAlarm} 
                            />
                        )}
                    </div>
                </div>
            </div>

            <SaveRestart config={this.props.config} save="1" restart="1" alarm={this.props.alarm} />
        </>);
    }
};

export default Alarm;