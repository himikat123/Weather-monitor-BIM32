import React from "react";
import axios from 'axios';

class LoginFn extends React.Component {
    constructor(props) {
        super(props);
        this.state = {
            show: false,
            name: '',
            pass: '',
            error: '',
            logining: ''
        };

        this.showHide = this.showHide.bind(this);
        this.changedName = this.changedName.bind(this);
        this.changedPass = this.changedPass.bind(this);
        this.login = this.login.bind(this);
        this.keyDown = this.keyDown.bind(this);
    };

    /**
     * shows and hides password
     */
    showHide() {
        this.setState({ show: !this.state.show });
    }

    /**
     * Username was entered
     */
    changedName(event) {
        this.setState({ name: event.target.value });
    }

    /**
     * Password was entered
     */
    changedPass(event) {
        this.setState({ pass: event.target.value });
    }

    /**
     * Login button was clicked
     */
    login() {
        this.setState({logining: 'spinner-border spinner-border-sm ms-2'});
        let origin = window.location.origin;
        let href = [origin.split(':')[0], origin.split(':')[1]].join(':');
        axios({
            method: 'get',
            url: `${href}/esp/login?name=${this.state.name}&pass=${this.state.pass}`
        })
        .then(
            res => this.setState({ error: res.data, logining: res.data == 'error' ? '' : 'spinner-border spinner-border-sm ms-2'})
        )
        .catch(() => this.setState({ error: 'network', logining: '' }))
    }

    /**
     * Login by pressing enter in the input field
     */
    keyDown(event) {
        if(event.keyCode == 13) this.login();
    }
}

export default LoginFn;