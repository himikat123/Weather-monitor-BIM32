import React from "react";
import Languages from "../Languages";
import axios from 'axios';
import ModalRestart from "../modals/ModalRestart";

class SaveRestart extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            recipes: '',
            saveButtonColor: 'btn-primary'
        };
        this.saveConfig = this.saveConfig.bind(this);
    };

    /**
     * change save button color
     */
    buttonColor() {
        if(this.state.recipes && this.state.recipes == 'OK') 
            this.setState({ saveButtonColor: 'btn-success' });
        else this.setState({ saveButtonColor: 'btn-danger' });
        setTimeout(() => {
            this.setState({ saveButtonColor: 'btn-primary' });
        }, 3000);
    }

    /**
     * save config
     */
    saveConfig() {
        this.setState({ saveButtonColor: 'btn-warning' });
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios({
            method: 'post',
            mode: 'cors',
            url: this.props.pass ? 
                `${href}/esp/changePass` : 
                this.props.alarm ? 
                    `${href}/esp/saveAlarm` : 
                    `${href}/esp/saveConfig`,
            data: this.props.pass ?
                `oldPass=${this.props.pass_old}&newPass=${this.props.pass_new}` :
                this.props.alarm ?
                `alarm=${JSON.stringify(this.props.alarm, null, '\t')}` :
                `config=${JSON.stringify(this.props.config, null, '\t')}`
        })
        .then(
            res => {
                this.setState({ recipes: res.data });
                this.buttonColor();
            }
        );
    }

    render() {
        let text = new Languages();
        let lang = this.props.config.lang;

        return (
            <div className="d-flex justify-content-center">
                {this.props.save &&
                    <button className={"btn m-3 mb-5 " + this.state.saveButtonColor} 
                        onClick={this.saveConfig}>
                            {text.get('save', lang)}
                    </button>
                }

                {this.props.restart && <div>
                    <button className="btn btn-secondary m-3 mb-5" 
                        data-bs-toggle="modal" 
                        data-bs-target="#modalRestart">
                            {text.get('restart', lang)}
                        </button>
                        <ModalRestart lang={this.props.config.lang} />
                    </div>
                }
            </div>
        );
    };
};

export default SaveRestart;