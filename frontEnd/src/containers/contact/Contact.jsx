import { Typography } from '@mui/joy';
import * as React from 'react';
import { NavBar } from '../../components';
import { Box } from '@mui/system';



const Contact = () => {

        return (
                <div className='bar'>
                        <NavBar></NavBar>
                        <Box className="container" sx={{ display: 'flex', justifyContent: 'flex-start', width: '75%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto' }}>

                                <Typography level='h1' color='info' textAlign={'center'}>This is the contact us page</Typography>
                                <Typography level='h2' color='warning' textAlign={'center'}>welcome!</Typography>
                                <Typography level='h3' color='danger' textAlign={'center'} sx={{ marginTop: "250px" }}>you can contact me at this email: barjonathanarie@gmail.com</Typography>
                                <Typography level='h3' color='danger' textAlign={'center'}>or you can add me on discord at Gangoofy#0914</Typography>
                        </Box>



                </div>

        );

}
export default Contact
