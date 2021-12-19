package app.android.adapter;

import android.view.LayoutInflater;
import android.view.ViewGroup;

import androidx.recyclerview.widget.RecyclerView;

import app.android.R;
import app.android.holder.ImageHolder;
import app.android.holder.LuzHolder;
import app.android.holder.TemperaturaHolder;
import app.android.holder.UmidadeArHolder;
import app.android.holder.UmidadeSoloHolder;

public class MainRecyclerAdapter extends RecyclerView.Adapter
{
    private final int LUZ_VIEW = 0, SOLO_VIEW = 1, TEMP_VIEW = 2, AR_VIEW = 3, IMAGE_VIEW = 4;

    public MainRecyclerAdapter()
    {
    }

    @Override
    public int getItemViewType(int position)
    {
        if(position == 0)
            return LUZ_VIEW;
        else if(position == 1)
            return SOLO_VIEW;
        else if(position == 2)
            return TEMP_VIEW;
        else if (position == 3)
            return AR_VIEW;
        else if(position == 4)
            return IMAGE_VIEW;
        else
            return -1;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType)
    {
        if(viewType == LUZ_VIEW) {
            LuzHolder luzHolder = new LuzHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.lus_sub_layout, parent, false));
            return luzHolder;
        }
        if(viewType == TEMP_VIEW) {
            TemperaturaHolder temperaturaHolder = new TemperaturaHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.temperatura_sub_layout, parent, false));
            return temperaturaHolder;
        }
        if(viewType == SOLO_VIEW) {
            UmidadeSoloHolder umidadeSoloHolder = new UmidadeSoloHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.umidade_solo_layout, parent, false));
            return umidadeSoloHolder;
        }
        if(viewType == AR_VIEW) {
            UmidadeArHolder umidadeArHolder = new UmidadeArHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.umidade_ar_layout, parent, false));
            return umidadeArHolder;
        }

        ImageHolder imageHolder = new ImageHolder(LayoutInflater.from(parent.getContext()).inflate(R.layout.image_sub_layout, parent, false));
        return imageHolder;
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position)
    {
    }

    @Override
    public int getItemCount()
    {
        return 5;
    }
}
