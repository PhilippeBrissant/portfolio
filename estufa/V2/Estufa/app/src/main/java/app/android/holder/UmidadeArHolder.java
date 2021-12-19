package app.android.holder;

import android.view.View;
import android.widget.TextView;

import androidx.recyclerview.widget.RecyclerView;

import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import app.android.R;

public class UmidadeArHolder extends RecyclerView.ViewHolder {
    private TextView tvTituloUmidade;
    private TextView tvSubUmidade;

    public UmidadeArHolder(View itemView) {
        super(itemView);
        tvTituloUmidade = (TextView) itemView.findViewById(R.id.tvTituloUmidadeAR);
        tvSubUmidade = (TextView) itemView.findViewById(R.id.tvSubUmidadeAR);
        final DatabaseReference database = FirebaseDatabase.getInstance().getReference().child("realtime");

        ValueEventListener listener = new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot dataSnapshot) {
                float temperatura = dataSnapshot.child("temperatura").getValue(Float.class);
                float umidade = dataSnapshot.child("umidade_ar").getValue(Float.class);

                float umidadeAr = dataSnapshot.child("umidade_ar").getValue(Float.class);
                tvTituloUmidade.setText(umidadeAr + "%");

                //source: https://betterorganix.com/blog/what-is-how-to-calculate-vapour-pressure-deficit/
                double calculatedVPD = 0;
                double VPleaf = 610.7 * Math.pow(10, (7.5 * temperatura) / (237.3 + temperatura));
                VPleaf = VPleaf / 1000;
                double VPair = VPleaf * (1 - umidade / 100);
                calculatedVPD = VPleaf - VPair;

                tvSubUmidade.setText("VPD está em " + String.format("%.1f", calculatedVPD) );
            }

            @Override
            public void onCancelled(DatabaseError databaseError) {
//                tvTituloLuz.setText(databaseError.getMessage());

            }
        };

        database.addValueEventListener(listener);
    }
}
