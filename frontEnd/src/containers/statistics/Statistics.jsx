import React, { useEffect, useState } from 'react';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, Legend, LineChart, Line, Label } from 'recharts';
import axios from 'axios';
import { NavBar } from '../../components'
import { Box } from '@mui/joy';

const Statistics = () => {
    const [data, setData] = useState([])
    const [lastRunStats, setLastRunStats] = useState([])
    useEffect(() => {

        axios.get('http://localhost:5000/Statistics')
            .then(response => {

                const mappedData = response.data.map(d => ({
                    fileSize: d.file_size,
                    overAllTime: d.saved_time - d.first_detected_time
                }));
                setData(mappedData);
            })
            .catch(error => {
                console.log(error);
            });

        axios.get('http://localhost:5000/getLatestRun')
            .then(response => {
                console.log("incoming data: " + response.data)
                const mappedData = response.data.map(d => ({
                    encodeTime: d.end_encode_time - d.start_encode_time,
                    decodeTime: d.recovered_time - d.first_time_detected,
                    sendTime: d.last_time_detected - d.mount_time,
                    entireTime: d.saved_time - d.first_detected_time,
                    fileSize: d.file_size,
                    blockSize: d.block_size,
                    chunkSize: d.chunk_size,
                    avg: d.avg_time

                }))
                setLastRunStats(mappedData);
            })
            .catch(error => {
                console.log(error);
            });

    }, []);

    return (
        <div className='.bdy'>
            <NavBar />
            <Box className="container" sx={{ display: 'flex', justifyContent: 'flex-start', width: '75%', height: '120%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto', marginTop: '100px' }}  >
                <LineChart width={1000} height={500} data={data} margin={{ left: 100, right: 100, bottom: 100 }}>
                    <XAxis dataKey="fileSize" >
                        <Label value="File Size (Bytes)" offset={-20} position="insideBottom" />                    </XAxis>
                    <YAxis >

                        <Label value="Total Time (millisecond)" offset={-20} position="insideLeft" angle={-90} />
                    </YAxis>
                    <CartesianGrid strokeDasharray="3 3" />
                    <Tooltip />

                    <Line type="monotone" dataKey="overAllTime" stroke="#000000" strokeWidth={4} strokeDasharray="1 0" strokeDashoffset={0} />
                </LineChart>



                <BarChart width={1000} height={500} margin={{ left: 50, right: 50, bottom: 100 }} data={lastRunStats} barGap={'20'}>
                    <XAxis />
                    <YAxis scale="log" domain={[1, "auto"]} >
                        <Label value="Size (Bytes)" offset={-20} position="insideLeft" angle={-90} />

                    </YAxis>
                    <CartesianGrid strokeDasharray="3 3" />
                    <Tooltip />
                    <Legend />
                    <Bar dataKey="fileSize" fill="#8884d8" />
                    <Bar dataKey="chunkSize" fill="#ffc658" />
                    <Bar dataKey="blockSize" fill="#82ca9d" />

                    {/* <Bar dataKey="entireTime" fill="#d84d29" /> */}
                </BarChart>
                <Box display={'flex'} flexDirection={'row'}>
                    <BarChart width={500} height={500} margin={{ left: 50, right: 50 }} data={lastRunStats} barGap={'20'}>
                        <XAxis /*dataKey="name"*/ />
                        <YAxis scale="log" type="number" domain={[1, 'dataMax+50']} >
                            <Label value="Time (millisecond)" offset={-20} position="insideLeft" angle={-90} />

                        </YAxis>
                        <CartesianGrid strokeDasharray="3 3" />
                        <Tooltip />
                        <Legend />
                        <Bar dataKey="encodeTime" fill="#000000" />
                        <Bar dataKey="decodeTime" fill="#ffc658" />
                        <Bar dataKey="sendTime" fill="#82ca9d" />
                    </BarChart>
                    <BarChart width={500} height={500} margin={{ left: 50, right: 50 }} data={lastRunStats} barGap={'50'}>
                        <XAxis /*dataKey="name"*/ />
                        <YAxis scale="log" type="number" domain={['dataMin-100', 'dataMax+50']} >
                            <Label value="Time (millisecond)" offset={-20} position="insideLeft" angle={-90} />

                        </YAxis>
                        <CartesianGrid strokeDasharray="3 3" />
                        <Tooltip />
                        <Legend />
                        <Bar dataKey="avg" fill="#000000" />
                        <Bar dataKey="entireTime" fill="#ffffff" barSize={100} />
                    </BarChart>
                </Box>

            </Box>
        </div>
    )
}

export default Statistics;
