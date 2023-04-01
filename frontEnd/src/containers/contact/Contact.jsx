import { Typography } from '@mui/joy';
import * as React from 'react';
import { NavBar } from '../../components';



const Contact = () => {

        return (
                <div>
                        <NavBar></NavBar>
                        <Typography level='h1' color='info' textAlign={'center'}>This is the contact us page</Typography>
                        <Typography level='h2' color='warning' textAlign={'center'}>welcome!</Typography>
                        <Typography level='h3' color='danger' textAlign={'center'} sx={{marginTop: "250px"}}>you can contact me at this email: barjonathanarie@gmail.com</Typography>
                        <Typography level='h3' color='danger' textAlign={'center'}>or you can add me on discord at Gangoofy#0914</Typography>




                </div>

        );

}
export default Contact
