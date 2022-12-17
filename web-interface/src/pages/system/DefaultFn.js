import Save from "../Save";
import { closeMenu } from "../../menu/closeMenu";
import axios from 'axios';
import defaultConfig from './defaultConfig.json'

class DefaultFn extends Save {
    constructor(props) {
        super(props);
        this.state = { 
            menuOpen: 1,
            recipes: '',
            saveButtonColor: 'btn-primary',
            config: defaultConfig
        }

        this.reset = this.reset.bind(this);
    }

    /**
     * Props have been updated 
     */
    componentDidUpdate() {
        if(this.state.menuOpen) {
            closeMenu();
            this.setState({ menuOpen: 0 });
        }
    }

    /**
     * Reset to default settings button has been pressed
     */
    reset() {
        this.setState({saveButtonColor: 'btn-warning'});
        axios({
            method: 'post',
            url: '/esp/saveConfig',
            data: `config=${JSON.stringify(this.state.config, null, '\t')}`
        })
        .then(
            res => this.setState({ recipes: res.data }),
            this.buttonColor(this.state.recipes)
        );
    }

    /**
     * Change reset to default settings button color
     * @param {*} answer 
     */
    buttonColor(answer){
        if(answer == 'OK') this.setState({saveButtonColor: 'btn-success'});
        else this.setState({saveButtonColor: 'btn-danger'});
        setTimeout(() => {
            this.setState({saveButtonColor: 'btn-primary'});
        }, 3000);
    }
};

export default DefaultFn;