import * as React from 'react';
import { Box, Tab, Tabs, TabList } from '@mui/joy/';
import { useLocation, useNavigate } from 'react-router-dom';

const NavBar = () => {
  const navigate = useNavigate();
  const location = useLocation();

  // Get the path name from the location object and remove the leading slash
  const pathname = location.pathname.slice(1);

  const [value, setValue] = React.useState(pathname);

  const handleChange = (event, newValue) => {
    setValue(newValue);
    navigate(`/${newValue}`);
  };

  return (
    <Box component="nav" aria-label="My site" sx={{ flexGrow: 1, width: '30%' }}>
      <Tabs value={value} onChange={handleChange} variant='solid' className='bar'>
        <TabList variant='solid' color='primary'>
          <Tab label="Home" value="" to="/" >Home Page</Tab>
          <Tab label="Transmitter" value="Transmitter" to="/Transmitter" >Transmitter</Tab>
          <Tab label="Receiver" value="Reciever" to="/Receiver" >Reciever</Tab>
          <Tab label="User Manual" value="Manual" to="/Manual" >User Manual</Tab>
          <Tab label="Contact Us" value="Contact" to="/Contact" >Contact Us</Tab>
        </TabList>
      </Tabs>
    </Box>
  );
};

export default NavBar;
