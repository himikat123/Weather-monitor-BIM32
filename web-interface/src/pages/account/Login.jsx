import LoginFn from "./LoginFn";
import Languages from "../../Languages";
import PageHeader from "../../fragments/PageHeader";
import { Navigate } from 'react-router-dom';

class Login extends LoginFn {
    render() {
        let text = new Languages();
        let lang = this.props.lang;
        
        return (
            <div className="d-flex justify-content-center">
                {this.props.data.hasOwnProperty('state') && this.props.data.state == 'OK' && <Navigate to='/' />}
                <div className="content m-2 m-lg-5">
                    <PageHeader h="2" text={text.get('weatherMonitorBIM32', lang)} />
                    <div className="row justify-content-center">
                        <div className="col border border-secondary rounded m-4 p-3 account">

                            {/* Username */}
                            <label className="form-label mt-3">
                                {text.get('username', lang)}
                            </label> 
                            <input type="text" 
                                className="form-control"
                                placeholder='username'
                                onChange={this.changedName}
                                onKeyDown={this.keyDown}
                            />

                            {/* Password */}
                            <label className="form-label mt-3">
                                {text.get('password', lang)}
                            </label>
                            <div className="input-group mb-3">
                                <input className="form-control" 
                                    placeholder='password'
                                    type={this.state.show ? 'text' : 'password'} 
                                    onChange={this.changedPass}
                                    onKeyDown={this.keyDown}
                                />
                    
                                <button className="input-group-text" 
                                    onClick={this.showHide} 
                                    title={text.get('showHide', lang)}
                                >
                                    <svg x="0px" y="0px" viewBox="0 0 1000 1000" width="23px">
                                        {this.state.show ?
                                            <path d="M989.9,501.9c0,0.4-0.1,0.8-0.1,1.2c0,0.2,0,0.4-0.1,0.6c0,0.3-0.1,0.6-0.1,0.9c0,0.3-0.1,0.7-0.1,1c0,0,0,0.1,0,0.1c-1.2,7.8-4.8,15-10.2,20.6c-10.3,12.5-21.5,24.2-32.8,35.7c-61.1,62-131.1,117.3-207.8,157.4c-49,25.6-101.4,46.3-155.7,56.2c-59.7,10.9-119.5,10.4-179-2.5c-102.8-22.4-196-77.9-277.4-144.3c-37.8-30.8-75-64.6-106.3-102.5c-13.7-16.6-13.7-36.2,0-52.8c10.3-12.5,21.5-24.2,32.8-35.6c61.1-62,131.1-117.3,207.8-157.4c49-25.6,101.4-46.3,155.7-56.3c59.7-10.9,119.5-10.4,179,2.5c102.8,22.4,196,77.9,277.4,144.3c37.8,30.8,75,64.6,106.3,102.5c5.5,5.6,9,12.9,10.2,20.6c0,0,0,0.1,0,0.1c0.1,0.3,0.1,0.7,0.1,1c0,0.3,0.1,0.6,0.1,0.9c0,0.2,0,0.4,0.1,0.6c0,0.4,0.1,0.8,0.1,1.2c0,0.6,0.1,1.2,0.1,1.9C990,500.6,990,501.3,989.9,501.9z M500,308.6c-101.5,0-183.7,82.3-183.7,183.7c0,101.5,82.3,183.7,183.7,183.7c101.5,0,183.7-82.3,183.7-183.7C683.7,390.9,601.5,308.6,500,308.6z M500,584.2c-50.7,0-91.9-41.1-91.9-91.9c0-50.7,41.1-91.9,91.9-91.9s91.9,41.1,91.9,91.9C591.9,543.1,550.7,584.2,500,584.2z"/> :
                                            <g transform="translate(0.000000,511.000000) scale(0.100000,-0.100000)">
                                                <path d="M4540.1,3168.2C2875.9,2995.8,1357.3,2028.8,250.4,439.3C45.5,144.4,47.5,92.7,271.5-223.3C677.5-794,1093.1-1244,1598.6-1657.6c80.4-67,145.5-126.4,145.5-132.1c0-5.8-216.4-153.2-478.8-325.5c-264.3-172.4-492.2-331.3-507.5-352.4c-55.5-78.5-34.5-164.7,72.8-319.8c109.2-157.1,181.9-197.3,271.9-157.1c24.9,11.5,1250.5,812,2723.2,1777.1C5298.4-200.3,7106.2,985.1,7843.5,1467.7c737.3,484.5,1359.7,900.1,1384.6,926.9c70.9,80.4,55.5,166.6-59.4,331.3c-101.5,147.5-164.7,189.6-249,166.6c-23-5.7-256.6-151.3-519-323.6l-476.8-314.1L7673,2407.9c-643.4,388.8-1292.6,626.2-2012.7,733.5C5430.6,3175.8,4768,3191.2,4540.1,3168.2z M5449.7,1904.3c218.3-57.5,509.4-204.9,689.4-350.4c136-111.1,293-273.9,275.8-289.2c-21.1-15.3-3025.7-1983.9-3031.5-1983.9c-17.2,0-132.1,281.5-168.5,413.6c-53.6,199.2-59.4,637.7-9.6,838.8c181.9,727.7,745,1262,1474.6,1401.8C4875.2,1971.3,5252.5,1957.9,5449.7,1904.3z"/>
                                                <path d="M7872.2,755.3c-561.1-369.6-1024.5-672.2-1026.4-676c-1.9-1.9-15.3-82.3-26.8-176.2c-99.6-756.4-632-1363.5-1369.2-1564.6c-222.2-59.4-660.7-59.4-884.7,0c-88.1,23-178.1,51.7-203,63.2c-40.2,17.2-116.8-26.8-723.9-423.2c-373.4-245.1-683.7-450-693.2-457.7c-28.7-28.7,501.7-220.2,857.9-310.2c1380.7-350.5,2836.1-80.4,4107.7,764.1c666.4,442.4,1332.9,1105,1838.4,1832.7c203,291.1,203,346.6-9.6,647.3c-252.8,358.1-773.7,970.9-823.5,970.9C8902.5,1425.5,8433.3,1122.9,7872.2,755.3z"/>
                                            </g>
                                        }
                                    </svg>
                                </button>
                            </div>

                            {/* Login button */}
                            <div className="text-center mt-4">
                                <button className="btn btn-primary" onClick={this.login}>
                                    {text.get('login', lang)}
                                    <span className={this.state.logining}></span>
                                </button>
                            </div>

                            {/* Error messages */}
                            {(this.state.error == 'error' || this.state.error == 'network') &&
                                <div className="border border-warning rounded text-center text-danger mt-4 py-1">
                                    {this.state.error == 'error' && text.get('wrongLoginOrPassword', lang)}
                                    {this.state.error == 'network' && text.get('networkError', lang)}
                                </div>
                            }
                        </div>
                    </div>
                </div>
                {this.state.error == 'OK' && <Navigate to='/' />}
            </div>
        );
    }
};

export default Login;