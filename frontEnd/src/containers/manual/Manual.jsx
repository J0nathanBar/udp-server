import * as React from 'react';
import { NavBar } from '../../components';
import { Tabs, TabList, Tab, TabPanel, Typography, Slider, Input, Button, FormHelperText } from '@mui/joy';
import { Box } from '@mui/system';
import { useState } from 'react';



const Manual = () => {
        const [slider1Value, setSlider1Value] = useState(25000);
        const [slider2Value, setSlider2Value] = useState(1300);
        const handleSlider1Change = (event, newValue) => {
                setSlider1Value(newValue);
        };
        const handleSlider2Change = (event, newValue) => {
                setSlider2Value(newValue);
        };


        return (
                <div>
                        <NavBar />
                        <Box className="container" sx={{ display: 'flex', justifyContent: 'flex-start', flexDirection: 'column' }}>
                                <Typography gutterBottom level='h1' color='primary' textAlign={'center'}>
                                        Welcome to the User Manual!
                                </Typography>
                                <Typography gutterBottom level='h2' color='info' textAlign={'center'}>
                                        Press on the tab that interests you to learn how to use it
                                </Typography>
                                <Tabs sx={{ width: '75%' }}>
                                        <TabList>
                                                <Tab>Transmitter</Tab>
                                                <Tab>Reciever</Tab>
                                        </TabList>
                                        <TabPanel value={0}>

                                                <Typography gutterBottom level='h3' color='primary' textAlign={'center'}>
                                                        This is the Transmitter, here you can send files to your destination!
                                                </Typography>
                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        When you send your files they will be split into chunks. This slider represents the size in bytes of each Chunk
                                                </Typography>
                                                <Slider
                                                        color='info'
                                                        defaultValue={25000}
                                                        style={{ width: 1000 }}
                                                        value={slider1Value}
                                                        onChange={handleSlider1Change}
                                                        valueLabelDisplay={"on"}
                                                        min={500}
                                                        max={100000}
                                                        step={500}
                                                        aria-labelledby="slider1"
                                                        size='lg'
                                                />
                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        The second step your file will go through after splitting is encoding. this will it get it split into even smaller blocks. this slider determines the size in bytes of those blocks
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='warning' textAlign={'center'}>
                                                        you may notice that the value changes sometimes when you move the first slider. That's because the Data blocks have to be smaller than the chunks
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='danger' textAlign={'center'}>
                                                        If your file is too small, for either of the splits the system will split it for you with a fitting size
                                                </Typography>
                                                <Slider
                                                        color='info'
                                                        defaultValue={1300}
                                                        value={slider2Value}
                                                        onChange={handleSlider2Change}
                                                        valueLabelDisplay={"on"}
                                                        min={100}
                                                        max={Math.min(slider1Value - 100, 9500)}
                                                        step={100}
                                                        size='lg'
                                                        aria-labelledby="slider2"
                                                />
                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        This is where you decide what to send! just put on the path of the directory or file you want to send and off you go.
                                                </Typography>
                                                <Input
                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
                                                        required
                                                        color='warning'
                                                        variant='soft'
                                                        placeholder='Insert Source directory here'
                                                        id="text-input"
                                                        label="Text input"

                                                        margin="normal" />
                                                <FormHelperText

                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
                                                >an example for a path could be: /home/jonny/Desktop/project/</FormHelperText>
                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        This button submits all of your data to the server. the moment you press it will start sending
                                                </Typography>
                                                <Button
                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}

                                                        color="success">
                                                        Submit
                                                </Button>
                                                <Typography gutterBottom level='h5' color='warning' textAlign={'center'}>
                                                        the system can handle changing the Configuration while sending your data
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='danger' textAlign={'center'}>
                                                        However it is recommended to put all of your files and directory into a single parent directory
                                                </Typography>
                                        </TabPanel>
                                        <TabPanel value={1}>
                                                <Typography gutterBottom level='h3' color='primary' textAlign={'center'}>
                                                        This is the Reciever, here you configure the destination for your!
                                                </Typography>
                                                <Typography gutterBottom level='h3' color='danger' textAlign={'center'}>
                                                        configure this side only if this PC is the destination of the data
                                                </Typography>

                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        This is where you decide what where your data will be saved! just put on the path of the directory and off you go.
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='warning' textAlign={'center'}>
                                                        The system will save it with the same hirrarchy that it was sent in!                                                </Typography>
                                                <Input
                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
                                                        required
                                                        color='warning'
                                                        variant='soft'
                                                        placeholder='Insert Source directory here'
                                                        id="text-input"
                                                        label="Text input"

                                                        margin="normal" />
                                                <FormHelperText

                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}
                                                >an example for a path could be: /home/jonny/Desktop/project/</FormHelperText>
                                                <Typography gutterBottom level='h4' color='info' textAlign={'center'}>
                                                        This button submits all of your data to the server. the moment you press it will start your data will be saved in the new location
                                                </Typography>
                                                <Button
                                                        sx={{ width: "50%", alignSelf: "center", marginLeft: "20%" }}

                                                        color="success">
                                                        Submit
                                                </Button>
                                                <Typography gutterBottom level='h5' color='warning' textAlign={'center'}>
                                                        the system can handle changing the Configuration while sending your data
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='danger' textAlign={'center'}>
                                                        However it is recommended to put all of your files and directory into a single parent directory
                                                </Typography>
                                                <Typography gutterBottom level='h5' color='danger' textAlign={'center'}>
                                                        old data will not be transferred only new files and folders comming in will be saved in your new destination!
                                                </Typography>
                                        </TabPanel>

                                </Tabs>
                        </Box>
                </div>

        );

}
export default Manual
