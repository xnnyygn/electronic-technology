package in.xnnyygn.bluetoothtool;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.support.v7.app.ActionBarActivity;
import android.os.Bundle;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.ListView;
import android.widget.Toast;

import java.util.UUID;


public class DeviceListActivity extends ActionBarActivity implements View.OnClickListener {

    private static final int REQUEST_ENABLE_BT = 1;
    private ArrayAdapter<DeviceSummary> mDeviceAdapter;
    private BluetoothAdapter mBluetoothAdapter;
    private Button mBtnScan;
    private Button mBtnStop;
    private static final UUID SPP_UUID = UUID.fromString("00000000-0000-1000-8000-00805F9B34FB");


    private class DeviceSummary {
        String name;
        String address;

        public DeviceSummary(String name, String address) {
            this.name = name;
            this.address = address;
        }

        public String toString() {
            if (name == null) {
                return address;
            }
            return name + '(' + address + ')';
        }
    }

    private final BroadcastReceiver mDeviceReceiver = new BroadcastReceiver() {
        public void onReceive(Context context, Intent intent) {
            String action = intent.getAction();
            if (BluetoothDevice.ACTION_FOUND.equals(action)) {
                BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
                mDeviceAdapter.add(new DeviceSummary(device.getName(), device.getAddress()));
                mDeviceAdapter.notifyDataSetChanged();
            }
        }
    };


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_device_list);

        // test if host device support bluetooth
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Device does not support Bluetooth", Toast.LENGTH_SHORT).show();
            finish();
            return;
        }

        // buttons
        mBtnScan = (Button) findViewById(R.id.btnScan);
        mBtnScan.setOnClickListener(this);
        mBtnStop = (Button) findViewById(R.id.btnStop);
        mBtnStop.setOnClickListener(this);
        setButtonStatus(false);

        // bluetooth device list
        ListView mLvDevices = (ListView) findViewById(R.id.lvDevices);
        mDeviceAdapter = new ArrayAdapter<DeviceSummary>(this, android.R.layout.simple_list_item_1);
        mLvDevices.setAdapter(mDeviceAdapter);
        mLvDevices.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> adapterView, View view, int i, long l) {
                onDeviceSelected(mDeviceAdapter.getItem(i));
            }
        });

        // register receiver for bluetooth
        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
        registerReceiver(mDeviceReceiver, filter);
    }

    @Override
    public void onClick(View view) {
        switch (view.getId()) {
            case R.id.btnScan:
                onBtnScanClicked();
                break;
            case R.id.btnStop:
                onBtnStopClicked();
                break;
        }
    }

    /**
     * Button scan was clicked.
     */
    private void onBtnScanClicked() {
        if (mBluetoothAdapter.isEnabled()) {
            scanDevice();
        } else {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        }
    }

    /**
     * Scan bluetooth devices.
     */
    private void scanDevice() {
        mDeviceAdapter.clear();
        mDeviceAdapter.notifyDataSetChanged();

        mBluetoothAdapter.startDiscovery();
        setButtonStatus(true);
    }

    /**
     * Button stop was clicked.
     */
    private void onBtnStopClicked() {
        mBluetoothAdapter.cancelDiscovery();
        setButtonStatus(false);
    }

    /**
     * Set button status.
     * @param discovering
     */
    private void setButtonStatus(boolean discovering) {
        mBtnScan.setEnabled(!discovering);
        mBtnStop.setEnabled(discovering);
    }

    /**
     * Device selected.
     *
     * @param item device
     */
    private void onDeviceSelected(DeviceSummary item) {
        // BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(item.address);
        startActivity(new Intent(this, DeviceActivity.class));
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case REQUEST_ENABLE_BT:
                if (resultCode == RESULT_OK) {
                    scanDevice();
                }
                break;
        }
    }

    @Override
    protected void onDestroy() {
        unregisterReceiver(mDeviceReceiver);

        super.onDestroy();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.device_list, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();
        if (id == R.id.action_settings) {
            return true;
        }
        return super.onOptionsItemSelected(item);
    }
}
