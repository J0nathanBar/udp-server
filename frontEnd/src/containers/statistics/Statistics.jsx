import React, { useEffect, useState } from 'react';
import { BarChart, Bar, XAxis, YAxis, CartesianGrid, Tooltip, Legend, LineChart, Label, Line } from 'recharts';
import axios from 'axios'; import { NavBar } from '../../components'
import { Box } from '@mui/joy';

const Statistics = () => {
    const [data, setData] = useState([]);
    useEffect(() => {
        axios.get('http://localhost:5000/Statistics')
            .then(response => {
                console.log(response.data)
                // Map over the data array and compute the y-axis values
                const mappedData = response.data.map(d => ({
                    xvar: d.block_size,
                    yvar: d.saved_time - d.first_detected_time
                }));
                setData(mappedData);
            })
            .catch(error => {
                console.log(error);
            });
    }, []);

    return (
        <div className='.bdy'>
            <NavBar />
            <Box className="container" sx={{ display: 'flex', justifyContent: 'flex-start', width: '75%', flexDirection: 'column', marginLeft: 'auto', marginRight: 'auto', marginTop: '100px' }}  >

                <LineChart width={500} height={300} data={data}>
                    <XAxis dataKey="xvar" />
                    <YAxis />
                    <CartesianGrid strokeDasharray="3 3" />
                    <Tooltip />
                    <Legend />
                    <Line type="monotone" dataKey="yvar" stroke="#000000" strokeWidth={4} strokeDasharray="1 0" strokeDashoffset={0} />
                </LineChart>
                <BarChart width={600} height={300} data={data}>
                    <CartesianGrid strokeDasharray="3 3" />
                    <XAxis dataKey="xvar" />
                    <YAxis />
                    <Tooltip />
                    <Legend />
                    <Bar dataKey="yvar" fill="#8884d8" />
                </BarChart>
            </Box>
        </div>
    )
}

export default Statistics