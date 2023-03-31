import * as React from 'react';
import { Typography } from '@mui/joy';
import { NavBar } from '../../components';


const Homepage = () => {
    return (
        <div>
            <NavBar />
            <Typography level='h1' gutterBottom
                color='primary'
                textAlign={'center'}>Welcome to the Virtual-Channel website</Typography>
            <Typography level='h2' gutterBottom
                color='info'
                textAlign={'center'}>Protect your network with one directional communication</Typography>
            <Typography level='h3' sx={{ marginTop: '600px' }} color='danger' textAlign={'center'}>
                Hop on to the Transmitter to send files!
            </Typography>
            <Typography level='h3' color='danger' textAlign={'center'}>

                Go over to Reciever if you're expecting files!
            </Typography>
            <Typography level='h3' color='danger' textAlign={'center'}>

                Use the Contact Us page to report bugs
            </Typography>
            <Typography level='h3'  color='danger' textAlign={'center'}>

                Or go over to the User manual for a detailed guide
            </Typography>


        </div>

    );

}
export default Homepage
