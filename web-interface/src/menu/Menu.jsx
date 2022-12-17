import React from "react";
import MenuStatus from "./MenuStatus";
import MenuNetwork from "./MenuNetwork";
import MenuSource from "./MenuSource";
import MenuClock from "./MenuClock";
import MenuAlarm from "./MenuAlarm";
import MenuDisplay from "./MenuDisplay";
import MenuSound from "./MenuSound";
import MenuComfort from "./MenuComfort";
import MenuHistory from "./MenuHistory";
import MenuSend from "./MenuSend";
import MenuReceive from "./MenuReceive";
import MenuSystem from "./MenuSystem";
import MenuAccount from "./MenuAccount";
import {ReactComponent as UpImage} from "../svg/up.svg";

class Menu extends React.Component {
	constructor(props) {
        super(props);
        this.state = {
            scrollUp: 'hide'
        }

        this.handleScroll = this.handleScroll.bind(this);
    }

	/**
     * Scroll top
     */
	scrollTop() {
        window.scrollTo({
            top: 0,
            left: 0,
            behavior: 'smooth'
        });
    }

	handleScroll() {
		let scroll = Math.round(window.scrollY);
		let scrollUp = (scroll > 200) ? '' : 'hide';
		this.setState({scrollUp: scrollUp});
	}

	componentDidMount() {
		window.addEventListener("scroll", this.handleScroll);
	}

	componentWillUnmount() {
		window.removeEventListener("scroll", this.handleScroll);
	}

	render() {
		return ( 
			<nav className="navbar navbar-expand-lg navbar-light menu">
				<div className="container-fluid">
					<button className="navbar-toggler collapsed" 
							id="mobile_menu_but"
							type="button" 
							data-bs-toggle="collapse" 
							data-bs-target="#navbarSupportedContent">
						<span className="navbar-toggler-icon"></span>
					</button>
					<div className="collapse navbar-collapse" id="navbarSupportedContent">
						<ul className="navbar-nav me-auto mb-2 mb-lg-0">
							<MenuStatus language={this.props.language}/>
							<MenuNetwork language={this.props.language} />
							<MenuSource language={this.props.language} />
							<MenuClock language={this.props.language} />
							<MenuAlarm language={this.props.language} />
							<MenuDisplay language={this.props.language} />
							<MenuSound language={this.props.language} />
							<MenuComfort language={this.props.language} />
							<MenuHistory language={this.props.language} />
							<MenuSend language={this.props.language} />
							<MenuReceive language={this.props.language} />
							<MenuSystem language={this.props.language} />
						</ul>
						<form className="d-flex">
							<MenuAccount language={this.props.language} />
						</form>
					</div>
				</div>

				<button className={"button-up " + this.state.scrollUp} onClick={this.scrollTop}><UpImage /></button>
			</nav>
		);
	}
};
export default Menu;