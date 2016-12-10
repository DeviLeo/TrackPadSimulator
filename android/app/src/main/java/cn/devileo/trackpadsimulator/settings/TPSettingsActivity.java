package cn.devileo.trackpadsimulator.settings;

import cn.devileo.trackpadsimulator.R;

import android.app.Activity;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

public class TPSettingsActivity extends Activity{

	//>> Static
	static final String LOG_TAG = "TrackPad_Settings";
	//<<
	
	//>> Variables
	EditText etHost;
	Button btnSave;
	Button btnCancel;
	//<<
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_settings);
		findAllViews();
		initAllViews();
	}
	
	void findAllViews(){
		etHost = (EditText)findViewById(R.id.etHost);
		btnSave = (Button)findViewById(R.id.btnSave);
		btnCancel = (Button)findViewById(R.id.btnCancel);
		
		btnSave.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				String pref_name = getResources().getString(R.string.pref_name);
				String pref_host_key = getResources().getString(R.string.tp_settings_pref_host);
				SharedPreferences sp = getSharedPreferences(pref_name, MODE_PRIVATE);
				sp.edit().putString(pref_host_key, etHost.getText().toString()).commit();
				Toast.makeText(TPSettingsActivity.this, R.string.save, Toast.LENGTH_SHORT).show();
				finish();
			}
		});
		
		btnCancel.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				finish();
			}
		});
	}
	
	void initAllViews(){
		String pref_name = getResources().getString(R.string.pref_name);
		String pref_host_key = getResources().getString(R.string.tp_settings_pref_host);
		SharedPreferences sp = getSharedPreferences(pref_name, MODE_PRIVATE);
		String host = sp.getString(pref_host_key, null);
		etHost.setText(host);
	}
}
